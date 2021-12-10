#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 fragPos;
out vec3 normal;

in vec3 norm[];

void main() {

	for (int i = 0; i < 3; i++) {
		normal = norm[i];
		gl_Position = gl_in[i].gl_Position;
		fragPos = gl_in[i].gl_Position.xyz;	
		EmitVertex();
	}
	
	EndPrimitive();
}
