#ifndef ME_GL_WINDOW
#define ME_GL_WINDOW
#include <QtOpenGL\qglwidget>
#include<QtGui/qkeyevent>
#include "Vector.h"

class MeGlWindow : public QGLWidget
{
protected:
	void initializeGL();
	void paintGL();
	void handleInput(QKeyEvent* event, bool pressed);

private:
	float deltaTime;
	float speed1, speed2;
	Vector2 v1, v2;
	Vector2 offset1, offset2;

	// Uniforms
	unsigned int colorId, scaleId, offsetId;

	void installShaders();

public:
};

#endif
