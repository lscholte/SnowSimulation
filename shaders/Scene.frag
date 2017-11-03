#version 150 core

in vec4 FragmentColor;
in vec4 FragmentWorldPosition;
in vec3 FragmentNormal;
in vec2 FragmentTextureCoords;
in vec4 SnowMapCoord;

uniform bool UseDiffuseMap;
uniform sampler2D DiffuseMap;
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

	vec3 diffuseTexture;
	if(UseDiffuseMap)
	{
		diffuseTexture = texture(DiffuseMap, FragmentTextureCoords).rgb;
	}
	else
	{
		diffuseTexture = FragmentColor.rgb;
	}

	vec3 color;
	if(FragmentNormal != vec3(0.0, 0.0, 0.0))
	{
		vec3 LightPosition = vec3(0.25, 0.5, 1);
		vec3 L = normalize(FragmentWorldPosition.xyz - LightPosition);
		vec3 N = normalize(FragmentNormal);

		float ambient = 0.5;
		float diffuse = max(dot(L, N), 0.0);

		color = diffuseTexture*ambient + diffuseTexture*diffuse;
		//FragColor = vec4(FragmentNormal, 1.0);
	}
	else
	{
		color = diffuseTexture;
	}

	if(snow < 0.1)
	{
		color = vec3(1.0, 1.0, 1.0);
	}

	FragColor = vec4(color, 1.0);
}
