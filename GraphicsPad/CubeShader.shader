#shader VERTEX
#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 normal;

uniform vec3 ambient;
uniform mat4 transformMat;
uniform vec3 lightPos;

out vec3 vecOutColor;
out vec3 myNormal;
out vec3 myPosition;

void main()
{
	vec4 p = vec4(position, 1.0);
	gl_Position = transformMat * p;
	//vecOutColor = vertexColor * ambient;
	myNormal = normal;
	myPosition = position;
}

#shader FRAGMENT
#version 430

uniform vec3 ambient;
uniform vec3 lightPos;

out vec4 drawColor;
in vec3 vecOutColor;
in vec3 myNormal;
in vec3 myPosition;

void main()
{
	vec3 lightVec = normalize(lightPos - myPosition);
	float brightness = max(dot(lightVec, myNormal), 0.0f) * 0.7f;
	vec3 c = vec3(brightness, brightness, brightness) + ambient;
	c = clamp(c, 0.0f, 1.0f);
	drawColor = vec4(c, 1.0f);
};