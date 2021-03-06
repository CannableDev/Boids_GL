#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 model;

out vec2 TexCoords;
out vec3 Normal;
out vec3 Position;
out vec4 FragPosLightSpace;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main() {
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    Position = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;    
    FragPosLightSpace = lightSpaceMatrix * vec4(Position, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}