#pragma once
#ifndef ME_GL_WINDOW
#define ME_GL_WINDOW

#include <QtOpenGL\qglwidget>


class MeGlWindow : public QGLWidget
{
protected:
	void initializeGL();
	void keyPressEvent(QKeyEvent* event);
	void keyReleaseEvent(QKeyEvent* event);
	void handleInput(QKeyEvent* event, bool pressed);
	void sendDataToOpenGL();
	void mouseMoveEvent(QMouseEvent* e);
	void paintGL();

public:
};

#endif
