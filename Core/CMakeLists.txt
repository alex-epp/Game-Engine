cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
set(CMAKE_CXX_STANDARD 17)

set(BASE_DIR ..)
set(RAPIDJSON ${BASE_DIR}/rapidjson/include)
set(LIB_HEADERS ${RAPIDJSON}/rapidjson/document.h) 
include_directories(${BASE_DIR}/rapidjson/include/)

project("Core")

set (SOURCES Constants.cpp Core.cpp FPSCounter.cpp Logger.cpp Message.cpp)
add_library(Core ${SOURCES} ${LIB_HEADERS})