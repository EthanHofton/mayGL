#ifndef entryPoint_hpp
#define entryPoint_hpp

#include "logger.hpp"
#include "window.hpp"

// reference the external function to be created by the user called create game
extern may::core::Window *may::core::createWindow();

int main(int argc, char** argv);

#endif