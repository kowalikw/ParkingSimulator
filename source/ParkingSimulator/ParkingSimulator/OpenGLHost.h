#ifndef OPEN_GL_HOST_H
#define OPEN_GL_HOST_H

#define GLEW_STATIC
#include "glew.h"
#include <QTime>
#include <QEvent>
#include <qdesktopwidget.h>
#include <QMouseEvent> 
#include <QOpenGLWidget>

#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

using namespace Qt;

class OpenGLHost : public QOpenGLWidget//, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	OpenGLHost(QWidget *parent);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent * event);

protected:
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();

	bool keys[1024];
	bool mouseFirstPressed = true;
	bool mouseLeftPressed = false;
	bool mouseRightPressed = false;
	bool mouseMiddlePressed = false;

	float widgetWidth;
	float widgetHeight;
	float pixelRatio;
	float mouseLastX;
	float mouseLastY;
	float mouseClickX;
	float mouseClickY;
	float mouseReleaseX;
	float mouseReleaseY;
};

#endif