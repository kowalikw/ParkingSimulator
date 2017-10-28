#ifndef OPEN_GL_HOST_H
#define OPEN_GL_HOST_H

#define GLEW_STATIC
#include "glew.h"
#include <QTimer>
#include <QEvent>
#include <qdesktopwidget.h>
#include <QMouseEvent> 
#include <QOpenGLWidget>

#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"
#include "NvgDrawHelper.h"

using namespace Qt;

class OpenGLHost : public QOpenGLWidget//, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	OpenGLHost(QWidget *parent);
	virtual void enterEvent(QEvent *event);
	virtual void leaveEvent(QEvent *event);
	virtual void mousePressEvent(QMouseEvent * event);
	virtual void mouseReleaseEvent(QMouseEvent * event);
	virtual void mouseMoveEvent(QMouseEvent * event);
	virtual void wheelEvent(QWheelEvent * event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent * event);

	void UpdateSettings();
protected:
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();

	void adjustMaxOffset();

	bool keys[1024];
	bool mouseFirstPressed = true;
	bool mouseLeftPressed = false;
	bool mouseRightPressed = false;
	bool mouseMiddlePressed = false;
	bool mouseInsideGLHost = false;

	float widgetWidth;
	float widgetHeight;
	float pixelRatio;
	float mouseLastX;
	float mouseLastY;
	float mouseClickX;
	float mouseClickY;
	float mouseReleaseX;
	float mouseReleaseY;
	float mouseOffsetX;
	float mouseOffsetY;
	float mouseWheelSteps;
	float magnificationRatio = 1.0f;

	glm::vec2 widgetSize;
	glm::vec2 widgetOffset;
	glm::vec2 maxWidgetOffset;
	glm::vec2 drawAreaSize;
	glm::vec2 drawAreaPosition;

	NVGcontext *vg = NULL;
	NvgDrawHelper *nvgHelper = NULL;

	const int EXTRA_OFFSET = 30;
	const float MIN_MAGNIFICATION_RATIO = 0.2;
	const float MAX_MAGNIFICATION_RATIO = 5.0;
};

#endif