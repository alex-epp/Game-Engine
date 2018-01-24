include_directories(${INCLUDES})
set (SOURCES main.cpp)

project("Game")
add_executable(Game ${SOURCES})

target_link_libraries (Game Core
                            PlayerController
                            RenderSystem
                            WindowContext
                            )
                            
target_link_libraries (Game assimp
                            glbinding
                            glfw3
                            sfml-graphics
                            X11
                            Xinerama
                            Xcursor
                            Xxf86vm
                            Xrandr
                            pthread
                            dl
                            )