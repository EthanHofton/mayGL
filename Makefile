UNAME_S = $(shell uname -s)

CC = c++

CXXFLAGS = -Werror -Wall -std=c++17 -Wc++11-extensions -Wvexing-parse -stdlib=libc++
CXXFLAGS += -Ilib/glm/ -Ilib/spdlog/include/ -Ilib/rapidjson/include/ -Ilib/fmt/include/ -Ilib/glew/include/
CXXFLAGS += -Ilib/SDL2/include/ -Ilib/stb_image/
CXXFLAGS += -Iinclude/
LDFLAGS = lib/glew/lib/libGLEW.a `sdl2-config --static-libs`

ifeq ($(UNAME_S), Darwin)
	LDFLAGS += -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa -framework CoreAudio
endif

SRC = $(wildcard src/**/*.cpp) $(wildcard lib/stb_image/*.cpp)
OBJ = $(addprefix $(BIN)/static/, $(notdir $(SRC:.cpp=.o)))
BIN = lib
PROGRAM = mayGL

.PHONY: all clean

all: dirs libs program

dirs:
	mkdir -p $(BIN)
	mkdir -p $(BIN)/static

libs:
	cd lib/spdlog && cmake . && make
	cd lib/fmt && cmake . && make
	cd lib/glew/auto && make && cd .. && make && make install
	cd lib/SDL2 && mkdir -p build && cd build && cmake .. && make

program: $(OBJ)
	ar rcs $(BIN)/static/lib$(PROGRAM).a $^

%.o: %.cpp
	$(CC) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -rf $(BIN)/static