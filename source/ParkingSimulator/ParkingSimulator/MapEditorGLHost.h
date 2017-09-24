#ifndef MAP_EDITOR_GL_HOST_H
#define MAP_EDITOR_GL_HOST_H

#include "OpenGLHost.h"
#include "MapEditor.h"
#include "GeometryHelper.h"
#include "Graph.h"
#include "Map.h"
#include "Path.h"
#include "PathPlanner.h"
#include "Simulation.h"
#include "NvgDrawHelper.h"

class MapEditorGLHost : public OpenGLHost
{
	Q_OBJECT

public:
	MapEditorGLHost(QWidget *parent);
	virtual void enterEvent(QEvent *event);
	virtual void leaveEvent(QEvent *event);
	virtual void mousePressEvent(QMouseEvent * event);
	virtual void mouseReleaseEvent(QMouseEvent * event);
	virtual void mouseMoveEvent(QMouseEvent * event);
	virtual void wheelEvent(QWheelEvent * event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent * event);
	void SetMapEditor(MapEditor *mapEditor);
	MapEditor* GetMapEditor();
protected:
	virtual void initializeGL() override;
	virtual void resizeGL(int w, int h) override;
	virtual void paintGL() override;
private:
	/*void adjustMaxOffset();*/
	void nvgRenderFrame();
	//void drawActiveElement();

	MapEditor *mapEditor;
	glm::vec2 positionOnMap;
	int offsetX = 0;
	int offsetY = 0;
	int maxOffsetX = 0;
	int maxOffsetY = 0;
	int mapWidth;
	int mapHeight;

	QCursor DEFAULT_CURSOR = Qt::ArrowCursor;
	QCursor HOVER_CURSOR = Qt::PointingHandCursor;
	QCursor MOVE_CURSOR = Qt::SizeAllCursor;
	QCursor ROTATE_CURSOR = Qt::OpenHandCursor;
	QCursor RESIZE_CURSOR = Qt::ArrowCursor;
};

#endif