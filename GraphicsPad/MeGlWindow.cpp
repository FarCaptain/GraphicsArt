#include <gl\glew.h>
#include <iostream>
#include <fstream>
#include <MeGlWindow.h>
#include <ShaderLoader.h>
using namespace std;

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

void sendDataToOpenGL()
{
	GLfloat verts[] =
	{
		+0.2f, -0.5f,
		+0.0f, +0.0f, +1.0f,

		+0.0f, +0.0f,
		+0.0f, +0.0f, +1.0f,

		-0.2f, -0.5f,
		+0.0f, +0.0f, +1.0f,


		+0.8f, -1.0f,
		+0.1f, +0.2f, +0.1f,
		+0.8f, +1.0f,
		+0.1f, +0.2f, +0.1f,
		+1.0f, +1.0f,
		+0.1f, +0.2f, +0.1f,
		+1.0f, -1.0f,
		+0.1f, +0.2f, +0.1f,
	};

	GLushort indices[] = { 0,1,2, 3,4,5,6 };


	GLuint myBufferID;
	glGenBuffers(1, &myBufferID);
	// both buffers?
	glBindBuffer(GL_ARRAY_BUFFER, myBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myBufferID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 7 + sizeof(indices), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(verts), verts);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vertex) * 7, sizeof(indices), indices);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	// color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char*)(sizeof(float) * 2));
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

void MeGlWindow::installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	ShaderSource temp = GetShaderSource("shader.shader");
	adapter[0] = temp.VertexSource.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	adapter[0] = temp.FragmentSource.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	//if( ! checkShaderStatus(vertexShaderID) || ! checkShaderStatus(fragmentShaderID))
	//	return;

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	//if ( ! checkProgramStatus(programID))
	//	return;

	colorId = glGetUniformLocation(programID, "color");
	scaleId = glGetUniformLocation(programID, "scale");
	offsetId = glGetUniformLocation(programID, "offset");

	glUseProgram(programID);
}

void MeGlWindow::initializeGL()
{
	offset1.x = 0.5f;
	offset1.y = 0.0f;
	offset2.x = -0.3f;
	offset2.y = 0.0f;

	v1.x = 0.1f;
	v1.y = 0.0f;
	v2.x = 0.0f;
	v2.y = 0.0f;

	speed1 = 0.6;
	speed2 = 0.6;

	glewInit();
	sendDataToOpenGL();
	installShaders();
}

void MeGlWindow::paintGL()
{
	deltaTime = 0.2f;
	offset1 = offset1 + v1 * deltaTime;
	offset2 = offset2 + v2 * deltaTime;

	glViewport(0, 0, width(), height());
	glClearColor(+0.9f, +0.8f, 0.8f, +1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// levelart
	glUniform3f(colorId, 0.1f, 0.3f, 0.1f);
	glUniform2f(scaleId, 1.0f, 1.0f);
	glUniform2f(offsetId, 0.0f, 0.0f);

	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_SHORT, (GLvoid*)(sizeof(Vertex) * 7 + sizeof(GLshort) * 3));

	// Player 1
	glUniform3f(colorId, 1.0f, 0.0f, 0.0f);
	glUniform2f(scaleId, 1, 1);
	glUniform2f(offsetId, offset1.x, offset1.y);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLvoid*)(sizeof(Vertex) * 7));

	// Player 2
	glUniform3f(colorId, 0, 0, 1);
	glUniform2f(scaleId, 0.5, 0.3);
	glUniform2f(offsetId, offset2.x, offset2.y);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLvoid*)(sizeof(Vertex) * 7));
}

void MeGlWindow::keyPressEvent(QKeyEvent* event) {
	handleInput(event, true);
}

void MeGlWindow::keyReleaseEvent(QKeyEvent* event) {
	handleInput(event, false);
}

void MeGlWindow::handleInput(QKeyEvent* event, bool pressed)
{
	float factor = pressed ? 1.0f : -0.0f;
	float forceFactor = 500;

	switch (event->key()) {
	case Qt::Key::Key_W: // W
		v1 = (Vector2{ 0, speed1 }*factor);
		break;
	case 0x0041: // A
		v1 = (Vector2{ -speed1, 0.0f }*factor);
		break;
	case 0x0053: // S
		v1 = (Vector2{ 0, -speed1 }*factor);
		break;
	case 0x0044: // D
		v1 = (Vector2{ speed2, 0 }*factor);
		break;
	case Qt::Key::Key_Up: // W
		v2 = (Vector2{ 0, speed2 }*factor);
		break;
	case Qt::Key::Key_Left: // A
		v2 = (Vector2{ -speed2, 0.0f }*factor);
		break;
	case Qt::Key::Key_Down: // S
		v2 = (Vector2{ 0, -speed2 }*factor);
		break;
	case Qt::Key::Key_Right: // D
		v2 = (Vector2{ speed2, 0 }*factor);
		break;
	}
	//std::cout << "{" << vel1.x << ", " << vel1.y << "}" << std::endl;
	repaint();
}