#version 460 core
layout (location = 0) out vec4 FragColour;
layout (location = 1) out vec4 BloomColour;

uniform vec3 colour;

void main()
{    
    FragColour = vec4(colour, 1);
    BloomColour = vec4(colour, 1) * 0.2;
}