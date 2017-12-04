#version 150 core

in vec4 FragmentColor;
in vec4 SnowMapCoord;
in vec3 FragmentNormal;
in vec4 FragmentWorldPosition;

uniform bool UseSnowMap;
uniform sampler2DShadow SnowMap;

out vec4 FragColor;

void main()
{
	//FragColor = FragmentColor;
	//return;


	float snow = 1.0f;
	if(UseSnowMap)
	{
		snow = textureProj(SnowMap, SnowMapCoord);
	}

	vec3 color = FragmentColor.rgb;
	float alpha = FragmentColor.a;
	if(snow < 0.1)
	{
		//color = (FragmentColor + 1.2*vec4(1.0, 1.0, 1.0, 1.0)) / 2.2;
		color = vec3(1.0, 1.0, 1.0);
		alpha = 1.0;
	}


	if(FragmentNormal != vec3(0.0, 0.0, 0.0))
	{
		vec3 LightPosition = vec3(15, 15, 15);
		vec3 L = normalize(FragmentWorldPosition.xyz - LightPosition);
		vec3 N = normalize(FragmentNormal);

		float ambient = 0.5;
		float diffuse = max(dot(L, N), 0.0);

		color = color*ambient + color*diffuse;
	}
	

	FragColor = vec4(color, alpha);
}
