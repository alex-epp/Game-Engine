# Game Engine

A partly finished game engine I work on occasionally. This is it rendering the Crytek Sponza model:
![screenshot](crytek-sponza.png)

### How to build (vcpkg and visual studio)
 * Install via vcpkg: assimp, glbinding, glfw3, glm, rapidjason and SFML.
 * Build using the .sln file provided

### How to build (Linux)
 * Clone the submodules for assimp, glbinding, glfw3, glm, rapidjason and SFML
 * Build with CMake
 * **Warning**: on my machine, the Linux build is currently broken displays only a black screen
