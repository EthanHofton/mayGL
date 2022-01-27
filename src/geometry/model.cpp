#include <mayGL/geometry/model.hpp>

namespace mayGL
{
    namespace geometry
    {
        Model::Model(std::string t_modelFilepath)
        {
            m_modelTransform = new component::Transform(this, "modelMeshesTransform");
            addComponent(m_modelTransform);
            loadFromFile(t_modelFilepath);

            for (auto mesh : m_meshes)
            {
                m_modelTransform->addMesh(mesh->getId());
            }
        }

        void Model::loadFromFile(std::string t_path)
        {
            m_modelOBJFile = "models/" + t_path;
            m_directory = m_modelOBJFile.substr(0, m_modelOBJFile.find_last_of('/') + 1);
            const aiScene* scene = aiImportFile(m_modelOBJFile.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

            if (!scene)
            {
                CORE_ERROR("model with path {} not found: {}", t_path, aiGetErrorString());
                return;
            }

            // load all meshes (assimp seperate a model into a mesh for each material)
            for (unsigned int meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++)
            {
                aiMesh *mesh = scene->mMeshes[meshIdx];

                std::vector<modelVertex> vertes;
                vertes.reserve(mesh->mNumVertices);

                aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
                aiColor4D ambientColor;
                aiColor4D diffuseColor;
                aiColor4D specularColor;
                float shininess;
                aiString materialName;

                aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambientColor);
                aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor);
                aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specularColor);
                aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess);
                aiGetMaterialString(material, AI_MATKEY_NAME, &materialName);

                std::string stringMatName = std::string(materialName.C_Str());

                for (unsigned int i = 0; i < mesh->mNumVertices; i++)
                {
                    aiVector3D vert = mesh->mVertices[i];
                    aiVector3D norm = mesh->mNormals[i];
                    glm::vec2 texCoords = glm::vec2(0);

                    if (mesh->mTextureCoords[0])
                    {
                        texCoords.x = mesh->mTextureCoords[0][i].x; 
                        texCoords.y = mesh->mTextureCoords[0][i].y;
                    }

                    vertes.push_back({ {vert.x, vert.y, vert.z}, {norm.x, norm.y, norm.z}, texCoords});
                }

                // get indices
                std::vector<unsigned int> indices;
                indices.reserve(mesh->mNumFaces * 3u);
                for (unsigned int i = 0; i < mesh->mNumFaces; i++)
                {
                    if (mesh->mFaces[i].mNumIndices != 3u)
                    {
                        CORE_CRITICAL("model not triangle detected...");
                        assert(mesh->mFaces[i].mNumIndices == 3u);
                    }

                    indices.push_back(mesh->mFaces[i].mIndices[0]);
                    indices.push_back(mesh->mFaces[i].mIndices[1]);
                    indices.push_back(mesh->mFaces[i].mIndices[2]);
                }

                // create mesh
                vertex::VertexLayout *vLayout = new vertex::VertexLayout;
                vLayout->push(new vertex::VertexComponent("pos", vertex::position, 3, GL_FLOAT, GL_FALSE, offsetof(modelVertex, m_pos)));
                vLayout->push(new vertex::VertexComponent("normal", vertex::normals, 3, GL_FLOAT, GL_FALSE, offsetof(modelVertex, m_normal)));
                vLayout->push(new vertex::VertexComponent("texCoords", vertex::texture_coords, 2, GL_FLOAT, GL_FALSE, offsetof(modelVertex, m_texCoords)));
                vLayout->push(new vertex::VertexComponent("Ka", vertex::Ka, 3, GL_FLOAT, GL_FALSE, offsetof(modelVertex, m_Ka)));
                vLayout->push(new vertex::VertexComponent("Kd", vertex::Kd, 3, GL_FLOAT, GL_FALSE, offsetof(modelVertex, m_Kd)));
                vLayout->push(new vertex::VertexComponent("Ks", vertex::Ks, 3, GL_FLOAT, GL_FALSE, offsetof(modelVertex, m_Ks)));
                vLayout->push(new vertex::VertexComponent("Ns", vertex::Ns, 1, GL_FLOAT, GL_FALSE, offsetof(modelVertex, m_Ns)));
                vLayout->push(new vertex::VertexComponent("map_Kd", vertex::map_Kd, 1, GL_FLOAT, GL_FALSE, offsetof(modelVertex, m_mapKd)));
                vLayout->push(new vertex::VertexComponent("map_Ks", vertex::map_Ks, 1, GL_FLOAT, GL_FALSE, offsetof(modelVertex, m_mapKs)));
                
                component::Mesh *matMesh = new component::Mesh(this, getEntityId() + "modelMesh:" + std::to_string(meshIdx), vLayout, GL_TRIANGLES);
                matMesh->setVertices(&vertes[0], sizeof(modelVertex) * vertes.size());
                matMesh->setIndices(&indices[0], sizeof(unsigned int) * indices.size());
                matMesh->loadShader("light.vertex", "light.fragment");
                addComponent(matMesh);

                if (!hasComponent(stringMatName))
                {
                    component::Material *meshMatieral = new component::Material(this, stringMatName);
                    meshMatieral->setAmbient(glm::vec3(ambientColor.r, ambientColor.g, ambientColor.b));
                    meshMatieral->setDiffuse(glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b));
                    meshMatieral->setSpecular(glm::vec3(specularColor.r, specularColor.g, specularColor.b));
                    meshMatieral->setShininess(shininess);

                    // diffuse, specular, bounce maps
                    auto diffuseMap = loadTexture(material, aiTextureType_DIFFUSE, stringMatName + ":map_Kd");
                    if (diffuseMap != nullptr)
                    {
                        CORE_INFO("PEEPEE");
                        addComponent(diffuseMap);
                        matMesh->addTexture(diffuseMap->getId());
                    }

                    // diffuse, specular, bounce maps
                    auto speuclarMap = loadTexture(material, aiTextureType_SPECULAR, stringMatName + ":map_Ks");
                    if (speuclarMap != nullptr)
                    {
                        addComponent(speuclarMap);
                        matMesh->addTexture(speuclarMap->getId());
                    }
                    

                    addComponent(meshMatieral);
                    meshMatieral->addMesh(matMesh->getId());
                } else {
                    auto meshMatieral = getComponent<component::Material, component::material>(stringMatName);
                    meshMatieral->addMesh(matMesh->getId());
                }

                m_meshes.push_back(matMesh);
            }
            
            aiReleaseImport(scene);
        }

        component::Texture* Model::loadTexture(aiMaterial *t_mat, aiTextureType t_type, std::string t_matName)
        {
            if (t_mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
            {
                aiString str;
                t_mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
                std::string path = std::string(str.C_Str());

                component::Texture *tex = new component::Texture(this, t_matName, m_directory + path);
                return tex;
            }

            return nullptr;
        }
    }
}