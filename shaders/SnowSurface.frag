#version 150 core

in vec4 FragmentWorldPosition;

void main()
{
    
    if(FragmentWorldPosition.y > 0.105)
    {
        discard;
    }
    
}