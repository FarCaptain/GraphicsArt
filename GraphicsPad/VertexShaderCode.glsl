#shader VERTEX
#version 430

in layout(location=0) vec2 position;
in layout(location=1) vec3 vertexColor;

out vec3 vecOutColor;

uniform vec3 color;
uniform vec2 scale;
uniform vec2 offset;

void main()
{
	gl_Position = vec4(scale*position + offset, 0.0, 1.0);
	vecOutColor = color;
}
;