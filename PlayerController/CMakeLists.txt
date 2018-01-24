include_directories(${INCLUDES})
set (SOURCES PlayerControllerSystem.cpp)

project("PlayerController")
add_library(PlayerController ${SOURCES})
