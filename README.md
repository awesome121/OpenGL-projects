# OpenGL projects
This repository stores some openGL projects. Before builing any specific projects, please make sure some toolkits</br> have been installed such as GLEW, freeglut.
Here is an example of how to install them on Linux.
## OpenGL on Linux
_[CMake](https://cmake.org/overview/) (used to build project)_
```
sudo apt-get install cmake
```
_[GLEW](http://glew.sourceforge.net) (OpenGL Extension Wrangler Library, can be used to determine if a specific function is supported)_
```
sudo apt-get install libglew-dev
```
_[freeglut](http://freeglut.sourceforge.net) (OpenGL utility library, it handle window, mouse, keyboard input and so on)_
```
sudo apt-get install freeglut3-dev
```
_[GLM](https://glm.g-truc.net/0.9.9/) (OpenGL mathematics library)_
```
sudo apt-get install libglm-dev
```

## Build
Change working directory to a project root directory.</br></br>
_Execute CMakeList.txt_
```
cmake .
```
_Build the project_
```
make
```
_Then run the corresponding executable (e.g. a.out)_
```
./a.out
```

## Screenshots
Some screenshots from [Synchronized Animation](https://github.com/awesome121/openGL-projects/tree/main/synchronized_animation):

<img src="/pendulum1.png" width=500 height=500>
<img src="/pendulum2.png" width=500 height=500>
<img src="/pendulum3.png" width=500 height=500>
