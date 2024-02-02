## General

Entity component system experiment for raylib. This has been mainly a study project.
Components don't have direct link to each other, they need to ask the component from the entity by it's type and it will return first one of that type it can find from it's component list. Most stuctures are stored in dynamic arrays that can be set to store any element size (same element size for all elements).

## Building

If compiling statically, move libraylib.a to "lib" folder. You could build with the following commands.

```
mkdir build
cd build
cmake ..
make
```
