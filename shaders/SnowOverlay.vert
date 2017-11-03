#version 150 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec4 Position;
layout(location = 3) in vec2 TextureCoords;

out vec2 FragmentTextureCoords;
out vec4 SnowMapCoord;

void main() {
    gl_Position = Position;

    FragmentTextureCoords = TextureCoords;
}