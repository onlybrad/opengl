# Pre-requisite
- Makefile
- CMake
- On Window: MinGW

# Dependencies
(all dependencies are included for convenience)
- cglm (C port of the C++ glm library, math library for vectors and matrices)
- glfw (Platform independant window library)
- stb_image (Loading images into memory)

# Building dependencies
```
make cglm && make glfw && make stb_image
```

# Building Windows debug executable 
(the debug executable will not work on a Windows machine without MinGW)
```
make main_debug.exe
```

# Building Windows release executable
```
make main.exe
```



