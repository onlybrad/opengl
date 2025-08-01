PHONY := cglm glfw stb_image
CC := gcc
STD := -std=c99
MACRO := -D GLFW_INCLUDE_NONE
RELEASE_MACRO := -D NDEBUG
INCLUDES := -isystem ./lib/cglm/include -I ./lib/glad/include -I ./lib/glfw/include -I ./lib/stb_image
FLAGS := -Wall -Wextra -Wpedantic -Wconversion -Wstrict-overflow=5 -Wshadow -Wunused-macros -Wbad-function-cast -Wcast-qual -Wcast-align -Wwrite-strings -Wdangling-else -Wlogical-op -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Winline
SOURCE := ./lib/glad/src/*.c ./src/core/opengl/*.c ./src/core/visual/*.c ./src/core/thread/*.c ./src/core/Object/*.c ./src/core/input/*.c ./src/core/util/*.c ./src/app/*.c
STATIC_LIBS := -L./lib/ -l:libglfw3.a -l:stb_image.a -l:libcglm.a
WINDOWS_DYNAMIC_LIBS := -lopengl32 -lgdi32

ifeq ($(OS),Windows_NT)
	CMAKE_TARGET := "MinGW Makefiles"
else
	CMAKE_TARGET := "Unix Makefiles" 
endif

main_debug.exe: $(SOURCE)
	$(CC) $(MACRO) $(INCLUDES) $(FLAGS) $(STD) -g -o main_debug $(SOURCE) $(STATIC_LIBS) $(WINDOWS_DYNAMIC_LIBS) -mconsole

main.exe: $(SOURCE)
	$(CC) $(MACRO) $(RELEASE_MACRO) $(INCLUDES) $(FLAGS) $(STD) -s -O3 -o main $(SOURCE) -static $(STATIC_LIBS) $(WINDOWS_DYNAMIC_LIBS) -mwindows

dependencies:
	make cglm && make glfw && make stb_image

cglm:
	mkdir -p ./lib/cglm/build
	cd ./lib/cglm/build && cmake -G $(CMAKE_TARGET) ..
	cd ./lib/cglm/build && make
	mv ./lib/cglm/build/libcglm.a ./lib

glfw:
	mkdir -p ./lib/glfw/build
	cd ./lib/glfw/build && cmake -G $(CMAKE_TARGET) ..
	cd ./lib/glfw/build && make
	mv ./lib/glfw/build/src/libglfw3.a ./lib

stb_image: ./lib/stb_image/stb_image.c ./lib/stb_image/stb_image.h
	$(CC) -I ./lib/stb_image -c -O3 -fPIC ./lib/stb_image/stb_image.c
	ar rcs ./lib/stb_image.a *.o
	rm -f *.o