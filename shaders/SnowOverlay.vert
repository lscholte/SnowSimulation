#version 150 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 Position;
layout(location = 2) in vec4 Color;

uniform mat4 ModelViewProjection;
uniform mat4 SkyMatrix;

out vec4 FragmentColor;
out vec4 SnowMapCoord;

void main()
{
	gl_Position = ModelViewProjection * vec4(Position, 1.0);
	
	FragmentColor = Color;
	SnowMapCoord = SkyMatrix * vec4(Position, 1.0);

}
