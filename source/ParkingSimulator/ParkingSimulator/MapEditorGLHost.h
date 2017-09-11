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
	float magnificationRatio = 1.0;
	int offsetX = 0;
	int offsetY = 0;
	int maxOffsetX = 0;
	int maxOffsetY = 0;
	int mapWidth;
	int mapHeight;
	int mapPositionX;
	int mapPositionY;

	const int SELECTED_MARKER_SIZE = 5;
	const NVGcolor SELECTED_MARKER_COLOR = nvgRGBA(255, 0, 0, 255);

	const int ACTIVE_BORDER_WIDTH = 3;
	const NVGcolor ACTIVE_GOOD_COLOR = nvgRGBA(135, 255, 145, 255);
	const NVGcolor ACTIVE_GOOD_BORDER_COLOR = nvgRGBA(0, 145, 0, 255);
	const NVGcolor ACTIVE_BAD_COLOR = nvgRGBA(255, 125, 125, 255);
	const NVGcolor ACTIVE_BAD_BORDER_COLOR = nvgRGBA(175, 0, 0, 255);

	const int EXTRA_OFFSET = 30;
	const float MIN_MAGNIFICATION_RATIO = 0.2;
	const float MAX_MAGNIFICATION_RATIO = 5.0;

	const int MAP_BORDER_WIDTH = 3;
	const NVGcolor MAP_COLOR = nvgRGBA(58, 124, 37, 255);
	const NVGcolor MAP_BORDER_COLOR = nvgRGBA(255, 0, 0, 255);

	const int BUILDING_BORDER_WIDTH = 3;
	const NVGcolor BUILDING_COLOR = nvgRGBA(140, 140, 140, 255);
	const NVGcolor BUILDING_BORDER_COLOR = nvgRGBA(160, 160, 160, 255);
};

#endif