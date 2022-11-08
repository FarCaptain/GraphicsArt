#version 430

// attribute 0, 1
in layout(location=0) vec2 position;
in layout(location=1) vec3 vertexColor;

out vec3 inColor;

void main()
{
	gl_Position = vec4(position, 0.0, 1.0);
	inColor = vertexColor;
}