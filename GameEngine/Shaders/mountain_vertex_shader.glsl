#version 400
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 textureCoord;
out vec3 norm;
out vec3 fragPos;

uniform mat4 MVP;
uniform mat4 model;

void main()
{
	textureCoord = aTexCoord;
	norm = mat3(transpose(inverse(model))) * aNormal;  
	fragPos = vec3(model * vec4(aPos, 1.0));
	gl_Position = MVP * vec4(aPos, 1.0);
}
