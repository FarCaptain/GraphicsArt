#include <gl\glew.h>
#include <ShaderLoader.h>
#include <iostream>
#include <MeGlWindow.h>
#include <QtGui\qmouseevent>
#include <QtGui\qkeyevent>
#include <MeGlWindow.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <Primitives\Vertex.h>
#include <Primitives\ShapeGenerator.h>
#include <Camera.h>

using namespace std;
using glm::vec3;
using glm::mat4;
using glm::translate;

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 6;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
GLuint programID;
GLuint numIndices;

float vx = 0;
float vy = 0;
glm::lowp_float angle = 0.0f;
Camera camera;

GLuint planeIndexByteOffset;
GLuint cubeIndexByteOffset;

GLuint planeNumIndices;
GLuint cubeNumIndices;

GLuint planeVertexArrayObjectID;
GLuint cubeVertexArrayObjectID;

void MeGlWindow::sendDataToOpenGL()
{
	ShapeData plane = ShapeGenerator::makePlane(20);
	ShapeData cube = ShapeGenerator::makeCube();

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER,
		plane.vertexBufferSize() + plane.indexBufferSize() +
		cube.vertexBufferSize() + cube.indexBufferSize(), NULL, GL_STATIC_DRAW);

	GLsizeiptr currentOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.vertexBufferSize(), plane.vertices);
	currentOffset += plane.vertexBufferSize();
	planeIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.indexBufferSize(), plane.indices);
	currentOffset += plane.indexBufferSize();

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.vertexBufferSize(), cube.vertices);
	currentOffset += cube.vertexBufferSize();
	cubeIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.indexBufferSize(), cube.indices);
	currentOffset += cube.indexBufferSize();

	planeNumIndices = plane.numIndices;
	cubeNumIndices = cube.numIndices;

	glGenVertexArrays(1, &planeVertexArrayObjectID);
	glGenVertexArrays(1, &cubeVertexArrayObjectID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBindVertexArray(planeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferID); //?

	GLuint cubeByteOffset = plane.vertexBufferSize() + plane.indexBufferSize();
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBindVertexArray(cubeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)cubeByteOffset);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(cubeByteOffset + sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(cubeByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferID);

	plane.cleanup();
	cube.cleanup();
}

void MeGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	mat4 Mt = glm::translate(mat4(), vec3(0.0f, 3.0f, 0.0f));
	//angle += vx;

	mat4 Mr = glm::rotate(mat4(), angle, vec3(1.0f, 0.0f, 0.0f));
	mat4 Mproj = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 100.f);

	mat4 Mx = Mproj * camera.getWorldToViewMatrix();//Mproj * Mt * Mr; //* Mt * Mr
	mat4 Mx1 = Mproj * Mt * Mr * camera.getWorldToViewMatrix();
	vec3 lightPosition(0.0f, 3.0f, 0.0f);
	vec3 camPosition = camera.getPosition();

	vec3 ambient(1.0f, 1.0f, 1.0f);
	vec3 specularColor(1.0f, 0.0f, 0.0f);
	float intensity = 0.5f;
	float ambientIntensity = 0.1f;
	
	GLint MxUniformLocation = glGetUniformLocation(programID, "transformMat");
	GLint lightPositionUniformLoca = glGetUniformLocation(programID, "lightPos");
	GLint camPosLocation = glGetUniformLocation(programID, "camPos");
	GLint KaLocation = glGetUniformLocation(programID, "Ka");
	GLint KsLocation = glGetUniformLocation(programID, "Ks");
	GLint IaLocation = glGetUniformLocation(programID, "Ia");
	GLint ILocation = glGetUniformLocation(programID, "I");

	glBindVertexArray(planeVertexArrayObjectID);
	glUniformMatrix4fv(MxUniformLocation, 1, GL_FALSE, &Mx[0][0]);
	glUniform3fv(lightPositionUniformLoca, 1, &lightPosition[0]);
	glUniform3fv(camPosLocation, 1, &camPosition[0]);
	glUniform3fv(KaLocation, 1, &ambient[0]);
	glUniform3fv(KsLocation, 1, &specularColor[0]);
	glUniform1f(IaLocation, ambientIntensity);
	glUniform1f(ILocation, intensity);
	glDrawElements(GL_TRIANGLES, planeNumIndices, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset);

	glBindVertexArray(cubeVertexArrayObjectID);
	glUniformMatrix4fv(MxUniformLocation, 1, GL_FALSE, &Mx[0][0]);
	glUniform3fv(lightPositionUniformLoca, 1, &lightPosition[0]);
	glUniform3fv(camPosLocation, 1, &camPosition[0]);
	glUniform3fv(KaLocation, 1, &ambient[0]);
	glUniform3fv(KsLocation, 1, &specularColor[0]);
	glUniform1f(IaLocation, ambientIntensity);
	glUniform1f(ILocation, intensity);
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);
}

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;
		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if ( ! meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}

void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	ShaderSource source = GetShaderSource("CubeShader.shader");
	const GLchar* adapter[1];
	adapter[0] = source.VertexSource.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	adapter[0] = source.FragmentSource.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if( ! checkShaderStatus(vertexShaderID) || ! checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if ( ! checkProgramStatus(programID))
		return;

	glUseProgram(programID);
}

void MeGlWindow::initializeGL()
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	installShaders();
}

void MeGlWindow::mouseMoveEvent(QMouseEvent* e)
{
	camera.mouseUpdate(glm::vec2(e->x(), e->y()));
	repaint();
}

void MeGlWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_W:
		camera.moveForward();
		break;
	case Qt::Key::Key_S:
		camera.moveBackward();
		break;
	case Qt::Key::Key_A:
		camera.strafeLeft();
		break;
	case Qt::Key::Key_D:
		camera.strafeRight();
		break;
	case Qt::Key::Key_R:
		camera.moveUp();
		break;
	case Qt::Key::Key_F:
		camera.moveDown();
		break;
	}
	repaint();
}