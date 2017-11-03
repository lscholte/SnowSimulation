#version 150 core

in vec2 FragmentTextureCoords;

uniform sampler2D SnowMap;

uniform mat4 InverseProjection;
uniform mat4 InverseView;

out vec4 FragColor;


vec4 positionFromDepth() {
    float depth = texture(SnowMap, FragmentTextureCoords).r * 2 - 1; //TODO Needs to be SnowMapTexCoords
    vec2 screenCoords = FragmentTextureCoords * 2.0 - 1.0;
    vec4 clipPos = vec4(screenCoords, depth, 1.0);
    vec4 viewPos = InverseProjection * clipPos;

    viewPos /= viewPos.w;

    vec4 pixelPos = InverseView * viewPos;

    return pixelPos;
}

void main(void)
{

    vec4 worldPos = positionFromDepth();

    if(worldPos.y < -10 || worldPos.y > 10)
    {
        discard;
    }

    FragColor = vec4(1.0, 1.0, 1.0, 1.0);


}