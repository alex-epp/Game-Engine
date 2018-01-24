include_directories(${INCLUDES})
set (SOURCES Constants.cpp Core.cpp FPSCounter.cpp Logger.cpp Message.cpp)

project("Core")
add_library(Core ${SOURCES})
