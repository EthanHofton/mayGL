#include <mayGL/component/materialComponent.hpp>
#include <mayGL/entity/entity.hpp>

namespace mayGL
{
    namespace component
    {
        Material::Material(entity::Entity *t_parent, std::string t_id, std::string t_materialName) : Component(t_parent, material, t_id)
        {
            m_ambient = glm::vec3(1, 0, 0);
            m_diffuse = glm::vec3(1, 0, 0);
            m_specular = glm::vec3(1, 0, 0);
            m_shininess = 32.0f;

            m_materialName = t_materialName;
        }

        void Material::setShaderUniforms()
        {
            for (auto mesh : m_meshes)
            {
                mesh->getShader()->addUniform(m_materialName + ".ambient");
                mesh->getShader()->addUniform(m_materialName + ".diffuse");
                mesh->getShader()->addUniform(m_materialName + ".specular");
                mesh->getShader()->addUniform(m_materialName + ".shininess");
            }
        }

        void Material::addMesh(std::string t_meshId)
        {
            m_meshes.push_back(getParent()->getComponent<Mesh, component::mesh>(t_meshId));
            CORE_INFO("Material with id '{}' added mesh with id '{}'", m_id, t_meshId);
        }

        void Material::addMeshes(std::vector<std::string> t_meshIds)
        {
            for (auto meshId : t_meshIds)
            {
                addMesh(meshId);
            }

            CORE_INFO("Material with id '{}' added {} meshes", m_id, t_meshIds.size());
        }

        void Material::addAllMeshes()
        {
            m_meshes.clear();
            m_meshes = getParent()->getComponents<Mesh, component::mesh>();

            CORE_INFO("Material with id '{}' added all meshes", m_id);
        }

        void Material::removeMesh(std::string t_meshId)
        {
            int index = 0;
            bool found = false;
            for (; index < m_meshes.size(); index++)
            {
                if (m_meshes[index]->getId() == t_meshId)
                {
                    found = true;
                    break;
                }
            }
            
            if (found)
            {
                CORE_INFO("Material with id '{}' removed mesh with id '{}'", m_id, t_meshId);
                m_meshes.erase(m_meshes.begin() + index);
            }
        }

        void Material::removeAllMeshes()
        {
            CORE_INFO("Material with id '{}' removed all bound meshes", m_id);
            m_meshes.clear();
        }

        void Material::update()
        {
            setShaderUniforms();

            for (auto mesh : m_meshes)
            {
                mesh->getShader()->setUniform3f(m_materialName + ".ambient", m_ambient);
                mesh->getShader()->setUniform3f(m_materialName + ".diffuse", m_diffuse);
                mesh->getShader()->setUniform3f(m_materialName + ".specular", m_specular);
                mesh->getShader()->setUniform1f(m_materialName + ".shininess", m_shininess);
            }
        }

        void Material::imguiComponentInspector()
        {
            // Component Info
            Component::imguiComponentInspector();
        }
    }
}