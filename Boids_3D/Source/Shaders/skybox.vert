#version 460 core
layout (location = 0) in vec3 vert;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = vert;    
    mat4 viewmat = mat4(mat3(transpose(inverse(view))));
    vec4 pos = projection * viewmat * vec4(vert, 1.0);
    gl_Position = pos.xyww;
}