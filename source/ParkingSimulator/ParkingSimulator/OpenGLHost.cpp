#include "OpenGLHost.h"

OpenGLHost::OpenGLHost(QWidget *parent) : QOpenGLWidget(parent) 
{
	setFocus();
	setMouseTracking(true);
	for (int i = 0; i < 1024; i++)
		keys[i] = false;
}

#pragma region User input events.

void OpenGLHost::enterEvent(QEvent * event)
{
	mouseInsideGLHost = true;
}

void OpenGLHost::leaveEvent(QEvent * event)
{
	mouseInsideGLHost = false;
}

void OpenGLHost::mousePressEvent(QMouseEvent *event)
{
	mouseLastX = event->x();
	mouseLastY = event->y();

	if (!mouseLeftPressed && !mouseRightPressed && !mouseMiddlePressed)
	{
		mouseClickX = event->x();
		mouseClickY = event->y();
	}

	if (event->button() == MouseButton::LeftButton)
		mouseLeftPressed = true;
	if (event->button() == MouseButton::RightButton)
		mouseRightPressed = true;
	if (event->button() == MouseButton::MiddleButton)
		mouseMiddlePressed = true;
	setFocus();
}

void OpenGLHost::mouseReleaseEvent(QMouseEvent * event)
{
	if (event->button() == MouseButton::LeftButton)
		mouseLeftPressed = false;
	if (event->button() == MouseButton::RightButton)
		mouseRightPressed = false;
	if (event->button() == MouseButton::MiddleButton)
		mouseMiddlePressed = false;

	if (!mouseLeftPressed && !mouseRightPressed && !mouseMiddlePressed)
	{
		mouseReleaseX = event->x();
		mouseReleaseY = event->y();
	}
}

void OpenGLHost::mouseMoveEvent(QMouseEvent * event)
{
	mouseOffsetX = event->x() - mouseLastX;
	mouseOffsetY = event->y() - mouseLastY;

	mouseLastX = event->x();
	mouseLastY = event->y();

	if (mouseMiddlePressed)
	{
		widgetOffset = glm::vec2(widgetOffset.x + mouseOffsetX / 2.0, widgetOffset.y + mouseOffsetY / 2.0);

		adjustMaxOffset();
	}
}

void OpenGLHost::wheelEvent(QWheelEvent * event)
{
	float numDegrees = event->delta() / 8.0f;
	mouseWheelSteps = numDegrees / 15.0f;

	event->accept();

	magnificationRatio += mouseWheelSteps / 20.0f;

	if (magnificationRatio < 0.05f)
		magnificationRatio = 0.05f;
}

void OpenGLHost::keyPressEvent(QKeyEvent * event)
{
	int key = event->key();
	if (key >= 0 && key < 1024)
		keys[key] = true;
}

void OpenGLHost::keyReleaseEvent(QKeyEvent * event)
{
	int key = event->key();
	if (key >= 0 && key < 1024)
		keys[key] = false;
}

#pragma endregion

#pragma region OpenGL methods.

void OpenGLHost::initializeGL()
{
	glewInit();

	vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
	nvgHelper = new NvgDrawHelper(vg, &widgetSize, &widgetOffset, &maxWidgetOffset, &drawAreaSize, &drawAreaPosition, &magnificationRatio);
}

void OpenGLHost::resizeGL(int w, int h)
{
	widgetWidth = w;
	widgetHeight = h;
	widgetSize = glm::vec2(w, h);
	pixelRatio = widgetWidth / widgetHeight;
	glViewport(0, 0, widgetWidth, widgetHeight);
}

void OpenGLHost::paintGL()
{
	glViewport(0, 0, widgetWidth, widgetHeight);
}

void OpenGLHost::adjustMaxOffset()
{
	if (widgetOffset.x > maxWidgetOffset.x + EXTRA_OFFSET)
		widgetOffset.x = maxWidgetOffset.x + EXTRA_OFFSET;
	if (widgetOffset.x < -maxWidgetOffset.x - EXTRA_OFFSET)
		widgetOffset.x = -maxWidgetOffset.x - EXTRA_OFFSET;
	if (widgetOffset.y > maxWidgetOffset.y + EXTRA_OFFSET)
		widgetOffset.y = maxWidgetOffset.y + EXTRA_OFFSET;
	if (widgetOffset.y < -maxWidgetOffset.y - EXTRA_OFFSET)
		widgetOffset.y = -maxWidgetOffset.y - EXTRA_OFFSET;
}

#pragma endregion

void OpenGLHost::UpdateSettings()
{
	if(nvgHelper != NULL)
		nvgHelper->UpdateSettings();
}