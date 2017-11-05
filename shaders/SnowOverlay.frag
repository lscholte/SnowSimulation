#version 150 core

in vec4 FragmentColor;
in vec4 SnowMapCoord;

uniform bool UseSnowMap;
uniform sampler2DShadow SnowMap;

out vec4 FragColor;

void main()
{

	float snow = 0.0f;
	if(UseSnowMap)
	{
		snow = textureProj(SnowMap, SnowMapCoord);
	}

	vec4 color = FragmentColor;
	if(snow < 0.1)
	{
		color = vec4(1.0, 1.0, 1.0, 1.0);
	}

	FragColor = color;
}
