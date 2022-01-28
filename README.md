## MayGL

A c++ SDL2 OpenGL GameEngine

# README UNDER CONSTRUCTION

## Install

###### INSTALL FOR OSX ONLY, GOOD LUCK BUILDING ON LINUX OR WINDOWS

To install mayGL, use the following commands:

```bash
git clone https://github.com/EthanHofton/mayGL.git
cd mayGL
make
```

This will create the static libary (`libmayGL.a`) in the `lib/static/` directory

To use the libary in your project, include the static libary and also the dependancys (OSX):

```
-Llib/mayGL/lib/static/ -lmayGL
-Llib/mayGL/lib/glew/ -lGLEW
-framework OpenGL
-rpath lib/mayGL/lib/SDL2/build/ lib/mayGL/lib/SDL2/build/libSDL2-2.0.dylib
```

Note: using SDL dylib on OSX creates issues due to @rpath, in order to fix this use `-rpath lib/mayGL/lib/SDL2/build/` flag in order to add the libary search path to the rpath search paths.

When building your application, link against all the libaries listed above aswell as adding `-Ilib/mayGL/include` to the header search paths

Also Note: building GLEW may require super user


