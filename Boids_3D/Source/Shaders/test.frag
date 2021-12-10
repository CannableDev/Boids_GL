#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

struct Material {
	vec3 diffuse;
	vec3 specular;
	float shininess;
	float reflective;
};

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;

uniform Material material;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform samplerCube skybox;

void main()
{    
    FragColor = vec4(material.diffuse, 1) * texture(texture_diffuse1, TexCoords);

	vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor += vec4(texture(skybox, R).xyz, 1.0) * material.reflective;
}