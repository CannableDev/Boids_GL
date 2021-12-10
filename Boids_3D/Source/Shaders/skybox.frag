#version 460 core
layout (location = 0) out vec4 FragColour;
layout (location = 1) out vec4 BloomColour;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColour = texture(skybox, TexCoords);
    
            float brightness = dot(FragColour.rgb, vec3(0.5, 0.5, 0.5));
    if(brightness > 1.0)
        BloomColour = vec4(FragColour.rgb + vec3(0.3,0,0.1),  1.0);
	else
		BloomColour = vec4(0.0, 0.0, 0.0, 1.0);
}