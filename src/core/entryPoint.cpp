#include <mayGL/core/entryPoint.hpp>

int main(int argc, char** argv)
{
    CORE_INFO("startup...");
    may::core::Window* game = may::core::createWindow();
    
    CORE_INFO("runtime...");
    game->run();
    
    CORE_INFO("cleanup...");
    delete game;
    game = nullptr;
}
