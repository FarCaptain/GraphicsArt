#shader VERTEX
#version 430

in layout(location = 0) vec4 vertexPositionModel;
in layout(location = 1) vec3 vertexColor;

out vec3 vertexToFragmentColor;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

void main()
{
	gl_Position = viewProjectionMatrix * modelMatrix * vertexPositionModel;
	vertexToFragmentColor = vertexColor;
}

#shader FRAGMENT
#version 430

in vec3 vertexToFragmentColor;
out vec4 daColor;

void main()
{
	daColor = vec4(vertexToFragmentColor, 1);
}