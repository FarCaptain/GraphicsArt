#version 430

out vec4 outColor;
in vec3 inColor;

void main()
{
	outColor = vec4(inColor, 1.0);
}