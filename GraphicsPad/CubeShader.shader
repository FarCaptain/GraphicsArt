#shader VERTEX
#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 normal;
in layout(location = 3) vec3 texCoord;

uniform vec3 ambient;
uniform mat4 modelMat[5];
uniform mat4 viewProjectionMat;
uniform vec3 lightPos;

out vec3 vecOutColor;
out vec3 myNormal;
out vec3 myPosition;
out vec2 myTexCoord;

void main()
{
	vec4 p = vec4(position, 1.0);
	mat4 Mm = modelMat[gl_InstanceID];
	gl_Position = viewProjectionMat * Mm * p;
	vecOutColor = vertexColor;
	// normal is a vector so ends with 0
	myNormal = normalize(vec3(Mm * vec4(normal, 0.0f)));
	myPosition = vec3(Mm * p);
	myTexCoord = vec2(texCoord);
}

#shader FRAGMENT
#version 430

uniform vec3 lightPos;
uniform vec3 camPos;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Ia;
uniform float I;
uniform bool normalMapFlag;

uniform sampler2D Tex;
uniform sampler2D DaNormalMap;

out vec4 drawColor;
in vec3 vecOutColor;
in vec3 myNormal;
in vec3 myPosition;
in vec2 myTexCoord;

void main()
{
	vec3 lightVec = normalize(lightPos - myPosition);
	vec3 camVec = normalize(camPos - myPosition);
	vec3 halfVec = normalize(lightVec + camVec);

	vec3 normal = myNormal;
	if(normalMapFlag)
		normal = texture(DaNormalMap, myTexCoord).xyz * 2.0f - 1.0f;

	float r = length(lightVec);
	float coe = I / (r * r);

	float diffuse = coe * max(dot(lightVec, normal), 0.0f);
	float specular = max(dot(halfVec, normal), 0.0f);

	vec3 texColor = vec3(texture(Tex, myTexCoord));
	vec3 c = vecOutColor * texColor * diffuse + Ks * coe * pow(specular, 16) + Ka * Ia;
	c = clamp(c, 0.0f, 1.0f);
	drawColor = vec4(c, 1.0f);
};