-- General
window_title = "Window title"
window_width = 640
window_height = 480
fullscreen = false

-- Rendering
FOV = 45
near_plane = 0.1
far_plane = 10000

-- Vertex shader input locations
vertex_location    = 0
texcoord_location  = 1
normal_location    = 2
tangent_location   = 3
bitangent_location = 4

-- MVP matrix
model_matrix_location = 0

-- Per-frame uniform block
perframe_uniform_name = "FrameData"
perframe_uniform_index = 0

-- Lighting. Because of stupid glsl stuff, this value isn't actually configurable. I left it here in case someone figures out how to fix this
max_lights = 8

-- Textures
texture_names = { "material.diffuseTexture", "material.specularTexture" , "material.shininessTexture" , "material.normalsTexture" }
texture_flags = { "material.diffuseTextureFlag", "material.specularTextureFlag", "material.shininessTextureFlag" , "material.normalsTextureFlag" }

-- Colours
colour_names = { "material.ambientColour", "material.diffuseColour", "material.specularColour" }

-- Other material attributes
shininess_name = "material.shininess"

-- Files
basic_shader = "basic"
shader_path = "../data/shaders/"