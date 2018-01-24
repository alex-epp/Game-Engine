include_directories(${INCLUDES})

set (SOURCES Material.cpp
             Mesh.cpp
             Model.cpp
             RenderSystem.cpp
             ShaderProgram.cpp
             VAO.cpp
             Vertex.cpp)

project("RenderSystem")            
add_library(RenderSystem ${SOURCES})
