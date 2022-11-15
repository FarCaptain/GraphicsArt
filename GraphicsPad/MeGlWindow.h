#pragma once
#ifndef ME_GL_WINDOW
#define ME_GL_WINDOW
//#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Primitives\Vertex.h>
#include <Primitives\ShapeGenerator.h>
#include <ShaderLoader.h>
#include <QtOpenGL\qglwidget>
#include<QtGui/qkeyevent>

class MeGlWindow : public QGLWidget
{
protected:
	void initializeGL();
	void keyPressEvent(QKeyEvent* event);
	void keyReleaseEvent(QKeyEvent* event);
	void handleInput(QKeyEvent* event, bool pressed);
	void paintGL();

public:
};

#endif
