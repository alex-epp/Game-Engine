include_directories(${INCLUDES})
set (SOURCES WindowContext.cpp)

project("WindowContext")
add_library(WindowContext ${SOURCES} ${LIB_HEADERS})