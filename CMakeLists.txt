cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
set(CMAKE_CXX_STANDARD 14)
cmake_policy(SET CMP0015 NEW)

set(BASE_DIR ..)
set(GLBINDING ${BASE_DIR}/glbinding/source/glbinding/include)
set(RAPIDJSON ${BASE_DIR}/rapidjson/include)
set(GLM ${BASE_DIR}/glm)
set(SFML ${BASE_DIR}/SFML/include)
set(ASSIMP ${BASE_DIR}/assimp/include)
set(GLFW ${BASE_DIR}/glfw/include)
set(INCLUDES ${RAPIDJSON} ${GLM} ${SFML} ${ASSIMP} ${GLBINDING} ${GLFW})

#find_package(glfw3 3.2 REQUIRED)
#find_package(assimp 4.1 REQUIRED)
#find_package(SFML 2 CONFIG REQUIRED graphics)

link_directories(Core
                 PlayerController
                 RenderSystem
                 WindowContext
                 assimp/lib
                 glbinding
                 SFML/lib
                 glfw/src
                 )

add_subdirectory(Core)
add_subdirectory(PlayerController)
add_subdirectory(RenderSystem)
add_subdirectory(WindowContext)
add_subdirectory(Components bin)
