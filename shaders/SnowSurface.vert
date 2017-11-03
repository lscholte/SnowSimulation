#version 150 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 Position;

uniform mat4 ModelViewProjection;
uniform mat4 Model;

out vec4 FragmentWorldPosition;

void main()
{
    vec3 pos = Position;
    pos.y += 0.001;
    FragmentWorldPosition = Model * vec4(pos, 1.0);
	gl_Position = ModelViewProjection * vec4(pos, 1.0);
}
