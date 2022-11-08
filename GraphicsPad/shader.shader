#shader VERTEX
#version 430

in layout(location = 0) vec2 position;
in layout(location = 1) vec3 vertexColor;

out vec3 vecOutColor;

void main()
{
	gl_Position = vec4(position, 0.0, 1.0);
	vecOutColor = vertexColor;
};

#shader FRAGMENT
#version 430

out vec4 drawColor;
in vec3 vecOutColor;

void main()
{
	drawColor = vec4(vecOutColor, 0.5f);
};