#version 150 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 Color;
layout(location = 3) in vec2 TextureCoords;

uniform mat4 ModelViewProjection;
uniform mat4 Model;

out vec4 FragmentColor;
out vec4 FragmentWorldPosition;
out vec3 FragmentNormal;
out vec2 FragmentTextureCoords;

void main()
{
	gl_Position = ModelViewProjection * vec4(Position, 1.0);
	
	FragmentColor = vec4(Color, 1.0);
	FragmentWorldPosition = Model * vec4(Position, 1.0);
	FragmentNormal = Normal;
	FragmentTextureCoords = TextureCoords;
}
