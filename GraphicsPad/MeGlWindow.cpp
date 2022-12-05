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
GLuint passthroughID;
GLuint numIndices;

float vx = 0;
float vy = 0;
glm::lowp_float angle = 0.0f;
Camera camera;

GLuint planeIndexByteOffset;
GLuint cubeIndexByteOffset;
GLuint sphereIndexByteOffset;
GLuint arrowIndexByteOffset;

GLuint planeNumIndices;
GLuint cubeNumIndices;
GLuint sphereNumIndices;
GLuint arrowNumIndices;

GLuint planeVertexArrayObjectID;
GLuint cubeVertexArrayObjectID;
GLuint sphereVertexArrayObjectID;
GLuint arrowVertexArrayObjectID;

vec3 lightPosition(0.0f, 3.0f, 0.0f);

void MeGlWindow::sendDataToOpenGL()
{
	ShapeData plane = ShapeGenerator::makePlane(20);
	ShapeData cube = ShapeGenerator::makeCube();
	ShapeData sphere = ShapeGenerator::makeSphere();
	ShapeData arrow = ShapeGenerator::makeArrow();

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER,
		plane.vertexBufferSize() + plane.indexBufferSize() +
		sphere.vertexBufferSize() + sphere.indexBufferSize() +
		arrow.vertexBufferSize() + arrow.indexBufferSize() +
		cube.vertexBufferSize() + cube.indexBufferSize(), NULL, GL_STATIC_DRAW);

	GLsizeiptr currentOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.vertexBufferSize(), plane.vertices);
	currentOffset += plane.vertexBufferSize();
	planeIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.indexBufferSize(), plane.indices);
	currentOffset += plane.indexBufferSize();

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere.vertexBufferSize(), sphere.vertices);
	currentOffset += sphere.vertexBufferSize();
	sphereIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere.indexBufferSize(), sphere.indices);
	currentOffset += sphere.indexBufferSize();

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.vertexBufferSize(), arrow.vertices);
	currentOffset += arrow.vertexBufferSize();
	arrowIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.indexBufferSize(), arrow.indices);
	currentOffset += arrow.indexBufferSize();

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.vertexBufferSize(), cube.vertices);
	currentOffset += cube.vertexBufferSize();
	cubeIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.indexBufferSize(), cube.indices);
	currentOffset += cube.indexBufferSize();

	planeNumIndices = plane.numIndices;
	cubeNumIndices = cube.numIndices;
	sphereNumIndices = sphere.numIndices;
	arrowNumIndices = arrow.numIndices;

	glGenVertexArrays(1, &planeVertexArrayObjectID);
	glGenVertexArrays(1, &cubeVertexArrayObjectID);
	glGenVertexArrays(1, &sphereVertexArrayObjectID);
	glGenVertexArrays(1, &arrowVertexArrayObjectID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBindVertexArray(planeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferID); //?

	GLuint sphereByteOffset = plane.vertexBufferSize() + plane.indexBufferSize();
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBindVertexArray(sphereVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)sphereByteOffset);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sphereByteOffset + sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sphereByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferID);

	GLuint arrowByteOffset = sphereByteOffset + sphere.vertexBufferSize() + sphere.indexBufferSize();
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBindVertexArray(arrowVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)arrowByteOffset);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(arrowByteOffset + sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(arrowByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferID);

	GLuint cubeByteOffset = arrowByteOffset + arrow.vertexBufferSize() + arrow.indexBufferSize();
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
	sphere.cleanup();
	arrow.cleanup();
	cube.cleanup();
}

void MeGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	mat4 Mt = glm::translate(mat4(), vec3(0.0f, 0.0f, 0.0f));

	mat4 Mr = glm::rotate(mat4(), angle, vec3(1.0f, 0.0f, 0.0f));
	mat4 Mproj = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 100.f);
	mat4 Ms = glm::scale(vec3(1.0f, 1.0f, 1.0f));

	mat4 Mvp = Mproj * camera.getWorldToViewMatrix();

	mat4 Mx[5];
	Mx[0] = Mt * Ms;
	vec3 camPosition = camera.getPosition();

	vec3 ambient(1.0f, 1.0f, 1.0f);
	vec3 specularColor(1.0f, 0.0f, 0.0f);
	float intensity = 0.5f;
	float ambientIntensity = 0.1f;
	
	GLint MxUniformLocation = glGetUniformLocation(programID, "modelMat");
	GLint MvpUniformLocation = glGetUniformLocation(programID, "viewProjectionMat");
	GLint lightPositionUniformLoca = glGetUniformLocation(programID, "lightPos");
	GLint camPosLocation = glGetUniformLocation(programID, "camPos");
	GLint KaLocation = glGetUniformLocation(programID, "Ka");
	GLint KsLocation = glGetUniformLocation(programID, "Ks");
	GLint IaLocation = glGetUniformLocation(programID, "Ia");
	GLint ILocation = glGetUniformLocation(programID, "I");

	// plane
	glBindVertexArray(planeVertexArrayObjectID);
	glUseProgram(programID);
	glUniformMatrix4fv(MxUniformLocation, 5, GL_FALSE, &Mx[0][0][0]);
	glUniformMatrix4fv(MvpUniformLocation, 1, GL_FALSE, &Mvp[0][0]);
	glUniform3fv(lightPositionUniformLoca, 1, &lightPosition[0]);
	glUniform3fv(camPosLocation, 1, &camPosition[0]);
	glUniform3fv(KaLocation, 1, &ambient[0]);
	glUniform3fv(KsLocation, 1, &specularColor[0]);
	glUniform1f(IaLocation, ambientIntensity);
	glUniform1f(ILocation, intensity);
	glDrawElements(GL_TRIANGLES, planeNumIndices, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset);

	/// Spheres
	glBindVertexArray(sphereVertexArrayObjectID);

	#pragma region calc Matrixs
	Mt = glm::translate(mat4(), vec3(-3, 3, 2)), Ms = glm::scale(vec3(0.2f, 0.2f, 0.2f));
	Mx[0] = Mt * Ms;

	Mt = glm::translate(mat4(), vec3(-1, 1, -0.5)), Ms = glm::scale(vec3(0.5f, 0.5f, 0.5f));
	Mx[1] = Mt * Ms;

	Mt = glm::translate(mat4(), vec3(4, 2, -1)), Ms = glm::scale(vec3(1.2f, 1.2f, 1.2f));
	Mx[2] = Mt * Ms;
	#pragma endregion
	glUniformMatrix4fv(MxUniformLocation, 3, GL_FALSE, &Mx[0][0][0]);
	glDrawElementsInstanced(GL_TRIANGLES, sphereNumIndices, GL_UNSIGNED_SHORT, (void*)sphereIndexByteOffset, 3);

	/// Arrows
	glBindVertexArray(arrowVertexArrayObjectID);
	Mt = glm::translate(mat4(), vec3(1, 1, 0.5));
	Ms = glm::scale(vec3(1.0f, 1.0f, 1.0f));
	Mx[0] = Mt * Ms;
	glUniformMatrix4fv(MxUniformLocation, 1, GL_FALSE, &Mx[0][0][0]);
	glDrawElements(GL_TRIANGLES, arrowNumIndices, GL_UNSIGNED_SHORT, (void*)arrowIndexByteOffset);

	/// Cube
	glBindVertexArray(cubeVertexArrayObjectID);
	Mt = glm::translate(mat4(), vec3(5, 1, 3));
	Ms = glm::scale(vec3(0.6f, 0.6f, 0.6f));
	Mx[0] = Mt * Ms;
	// the only thing changes at the moment is the MVP mat
	glUniformMatrix4fv(MxUniformLocation, 1, GL_FALSE, &Mx[0][0][0]);
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);

	/// light cube
	glBindVertexArray(cubeVertexArrayObjectID);
	glUseProgram(passthroughID);
	Mt = glm::translate(mat4(), lightPosition);
	Ms = glm::scale(vec3(0.1f, 0.1f, 0.1f));
	Mx[0] = Mt * Ms;
	GLint LightCubeMxUniformLocation = glGetUniformLocation(passthroughID, "modelMatrix");
	GLint LightCubeMvpUniformLocation = glGetUniformLocation(passthroughID, "viewProjectionMatrix");
	glUniformMatrix4fv(LightCubeMxUniformLocation, 1, GL_FALSE, &Mx[0][0][0]);
	glUniformMatrix4fv(LightCubeMvpUniformLocation, 1, GL_FALSE, &Mvp[0][0]);
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


	source = GetShaderSource("Passthrough.shader");
	adapter[0] = source.VertexSource.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	adapter[0] = source.FragmentSource.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	passthroughID = glCreateProgram();
	glAttachShader(passthroughID, vertexShaderID);
	glAttachShader(passthroughID, fragmentShaderID);
	glLinkProgram(passthroughID);

	//glUseProgram(passthroughID);
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
		//camera.moveForward();
		lightPosition = camera.moveVectorForward(lightPosition);
		break;
	case Qt::Key::Key_S:
		//camera.moveBackward();
		lightPosition = camera.moveVectorBackward(lightPosition);
		break;
	case Qt::Key::Key_A:
		//camera.strafeLeft();
		lightPosition = camera.moveVectorLeft(lightPosition);
		break;
	case Qt::Key::Key_D:
		//camera.strafeRight();
		lightPosition = camera.moveVectorRight(lightPosition);
		break;
	case Qt::Key::Key_R:
		//camera.moveUp();
		lightPosition = camera.moveVectorUp(lightPosition);
		break;
	case Qt::Key::Key_F:
		//camera.moveDown();
		lightPosition = camera.moveVectorDown(lightPosition);
		break;
	}
	repaint();
}