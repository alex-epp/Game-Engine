cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
set(CMAKE_CXX_STANDARD 14)

set(BASE_DIR ..)
set(RAPIDJSON ${BASE_DIR}/rapidjson/include)
set(GLM ${BASE_DIR}/glm)
include_directories(${RAPIDJSON} ${GLM})

set(LIB_HEADERS ${RAPIDJSON}/rapidjson/document.h)

project("Core")

set (SOURCES Constants.cpp Core.cpp FPSCounter.cpp Logger.cpp Message.cpp)
add_library(Core ${SOURCES} ${LIB_HEADERS})
