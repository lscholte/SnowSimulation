#version 150 core

in vec4 FragmentColor;
in vec4 SnowMapCoord;
in vec3 FragmentNormal;
in vec4 FragmentWorldPosition;
in vec2 FragmentTextureCoords;

uniform vec3 CameraPosition;
uniform vec3 LightPosition;

uniform bool UseSnowMap;
uniform sampler2DShadow SnowMap;

uniform bool UseNormalMap;
uniform sampler2D NormalMap;

out vec4 FragColor;

mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx(p);
    vec3 dp2 = dFdy(p);
    vec2 duv1 = dFdx(uv);
    vec2 duv2 = dFdy(uv);

    float r = 1.0f / (duv1.x * duv2.y - duv1.y * duv2.x);
    vec3 T = (dp1 * duv2.y   - dp2 * duv1.y)*r;
    vec3 B = (dp2 * duv1.x   - dp1 * duv2.x)*r;

    T = normalize(T);
    B = normalize(B);
    N = normalize(N);

    return mat3(T, B, N);
}

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
		vec3 L = normalize(FragmentWorldPosition.xyz - LightPosition);
		vec3 N = -normalize(FragmentNormal);
		vec3 V = normalize(CameraPosition - FragmentWorldPosition.xyz);

		if(UseNormalMap) {
			mat3 TBN = cotangent_frame(N, V, FragmentTextureCoords);
			N = normalize(2*texture(NormalMap, FragmentTextureCoords).xyz-1);
			N = normalize(TBN * N);
    	}

		float ambient = 0.5;
		float diffuse = max(dot(L, N), 0.0);
        float specular = 0.0;
		if(diffuse > 0.0) {
      		vec3 H = normalize(L + V);
      		float specAngle = max(dot(H, N), 0.0);
      		specular = pow(specAngle, 1.0);
    	}

		color = color*ambient + color*diffuse + specular*vec3(1.0, 1.0, 1.0);	
	}
	

	FragColor = vec4(color, alpha);
}
