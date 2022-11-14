#shader VERTEX
#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 vertexColor;

uniform mat4 transformMat;

out vec3 vecOutColor;

void main()
{
	vec4 p = vec4(position, 1.0);
	gl_Position = transformMat * p;
	vecOutColor = vertexColor;
}

#shader FRAGMENT
#version 430

out vec4 drawColor;
in vec3 vecOutColor;

void main()
{
	drawColor = vec4(vecOutColor, 0.5f);
};