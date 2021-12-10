#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;

uniform bool bloom;

void main()
{             
    const float gamma = 1.0;
    vec3 colour = texture(scene, TexCoords).rgb;      
    vec3 bloomColour = texture(bloomBlur, TexCoords).rgb;
    if(bloom)
        colour += bloomColour; // additive blending
    // tone mapping
    vec3 result = colour;
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
    
    FragColor = vec4(result, 1.0);
}