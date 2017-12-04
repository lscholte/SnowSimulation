#version 150 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec4 PositionAlpha;
layout(location = 1) in vec3 Normal;
layout(location = 3) in vec2 TextureCoords;

uniform mat4 ModelViewProjection;
uniform mat4 SkyMatrix;

out vec4 FragmentColor;
out vec4 SnowMapCoord;
out vec3 FragmentNormal;
out vec4 FragmentWorldPosition;
out vec2 FragmentTextureCoords;



void main()
{
	gl_Position = ModelViewProjection * vec4(PositionAlpha.xyz, 1.0);
	
	FragmentColor = vec4(1.0, 1.0, 1.0, PositionAlpha.a);
	SnowMapCoord = SkyMatrix * vec4(PositionAlpha.xyz, 1.0);

	FragmentNormal = Normal;
	FragmentWorldPosition = vec4(PositionAlpha.xyz, 1.0);
	FragmentTextureCoords = TextureCoords;

}
