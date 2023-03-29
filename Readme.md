# My OpenGL tests

This is a repository containing my personal OpenGL and Imgui tests.


### Requirements

CMake (major version 3)

This had been build on a Windows machine, so be aware that other operating systems may have some bugs, if you find them, create an issue

### Building

To build, just run the CMake command and compile the program
```bash
cmake -S . -B build/ -G "MinGW Makefiles";
cd ./build/;
mingw32-make
```
^ just make sure that you pass the right -G flag (build tool generator), and build call, depending on your setup

Binary file fill appear in the build/ folder

### Dependencies

All dependencies can be found in the submodules folder (and therefore you shouldn't have dependency related trouble building)

### Test list
- Clear color background
- 4 edge body (Rectangle)
- Texture 2D

### Credits

Thank you TheCherno for creating great tutorials to learn openGL from :DDD
https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
