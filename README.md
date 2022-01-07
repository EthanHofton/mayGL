## MayGL

A c++ SDL2 OpenGL GameEngine

## Usage

To use mayGL ypu can add it to your project normally or as a submodule. Listed below are the ways to do both

### Add to project as a git submodule:

create a folder in your project for libaries called lib

```bash
mkdir -p lib
```

add the submodule to the lib directory

```bash
git submodule add https://github.com/EthanHofton/mayGL.git lib/mayGL
git commit -m "mayGL added to project"
git submodule update --init --recercive
```

This will download the requred submodules for mayGL


### Add to project normally

```bash
mkdir -p lib
cd lib
bit clone --recurcive https://github.com/EthanHofton/mayGL.git lib/mayGL
```

### To use using g++:
add header seach path `lib/mayGL/include/`

```bash
CXXFLAGS += -Ilib/mayGL/include/
```
(add search path to the compilation stage)

add libary `lib/mayGL/lib/static/libmayGL.a` to linker stage
```bash
LDFLAGS += lib/mayGL/lib/static/libmayGL.a
```

*** make sure to call `cd lib/maycraft && make` before compiling ***

The GLEW and SDL2 libaries must be linked to your final project. You can do so in the makefile as follows:

```
LDFLAGS += lib/mayGL/lib/glew/lib/libGLEW.a `sdl2-config --static-libs`
```

## Example makefile for useage

```Makefile
UNAME_S = $(shell uname -s)

CC = c++

CXXFLAGS = -Wall -std=c++17 -stdlib=libc++
CXXFLAGS += -Ilib/glm/ -Ilib/mayGL/include/
LDFLAGS = lib/mayGL/lib/static/libmayGL.a
LDFLAGS += lib/mayGL/lib/glew/lib/libGLEW.a `sdl2-config --static-libs`
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
PROGRAM = out
BIN = bin

ifeq ($(UNAME_S), Darwin)
	LDFLAGS += -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa -framework CoreAudio
endif

.PHONY: all clean

all: dirs libs $(PROGRAM)

dirs:
	mkdir -p ./$(BIN)

libs:
	cd lib/mayGL && make

$(PROGRAM): $(OBJ)
	$(CC) -o $(BIN)/$(PROGRAM) $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CXXFLAGS)

run: all
	./$(BIN)/$(PROGRAM)

clean:
	rm -rf $(BIN) $(OBJ)
```

The Makefile above can be used in a project with the following structure

```

my-project/
├─ config/
│  ├─ gameConfig.json
│  ├─ loggerConfig.json
├─ lib/
│  ├─ mayGL/
├─ logs/
├─ res/
├─ shaders/
├─ src/
│  ├─ main.cpp
├─ Makefile
```
