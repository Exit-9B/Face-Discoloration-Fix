# Face Discoloration Fix

Removes dark faces and other discoloration issues from faulty FaceGen data.

## Requirements
* [CMake](https://cmake.org/)
	* Add this to your `PATH`
* [Vcpkg](https://github.com/microsoft/vcpkg)
	* Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
* [Visual Studio Community 2022](https://visualstudio.microsoft.com/)
	* Desktop development with C++

## Register Visual Studio as a Generator
* Open `x64 Native Tools Command Prompt`
* Run `cmake`
* Close the cmd window

## Building
```
git clone https://github.com/Exit-9B/Face-Discoloration-Fix
cd Face-Discoloration-Fix
git submodule init
git submodule update
cmake --preset vs2022-windows
cmake --build build --config Release
```
