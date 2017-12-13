#include "MapEditorGLHost.h"

MapEditorGLHost::MapEditorGLHost(QWidget *parent) : OpenGLHost(parent) 
{ 
	
}

#pragma region User input events.

void MapEditorGLHost::enterEvent(QEvent * event)
{
	OpenGLHost::enterEvent(event);
}

void MapEditorGLHost::leaveEvent(QEvent * event)
{
	OpenGLHost::leaveEvent(event);
}

void MapEditorGLHost::mousePressEvent(QMouseEvent * event)
{
	OpenGLHost::mousePressEvent(event);

	this->mouseMoveEvent(event);

	if (mapEditor->GetSelectedElement() != NULL && mapEditor->IsMapElementAdmissible(mapEditor->GetSelectedElement()))
	{
		int resizeCorner;
		MapElement *selectedElement = mapEditor->GetSelectedElement();
		selectedElement->EnableMove(false);
		selectedElement->EnableRotation(false);
		selectedElement->EnableResize(false);

		if (mapEditor->GetMapElementToMove(positionOnMap) == selectedElement)
			selectedElement->EnableMove(true);
		if (mapEditor->GetMapElementToRotate(positionOnMap) == selectedElement)
			selectedElement->EnableRotation(true);
		if (mapEditor->GetMapElementToResize(positionOnMap, &resizeCorner, selectedElement) == selectedElement || selectedElement->IsResizeActive())
		{
			selectedElement->EnableResize(true);
			selectedElement->SetResizeHoverCorner(resizeCorner);
		}
	}

	if (mapEditor->GetNewElement() == nullptr && mapEditor->GetHoverElement(positionOnMap) != NULL && !mapEditor->GetAddTerrain())
	{
		MapElement *selectedElement = mapEditor->GetSelectedElement();
		if (selectedElement == NULL ||
			(!selectedElement->IsMoveHover() && !selectedElement->IsRotationHover() && !selectedElement->IsResizeHover()))
			{
				mapEditor->SetSelectedElement(mapEditor->GetHoverElement(positionOnMap));
				mapEditor->SetSelectedElementChanged(true);
				mapEditor->SetMapElementsPropertiesChanged(true);
			}
		mapEditor->SetSelectedElementChangedOnMap(true);
	}
	else if (mapEditor->GetSelectedElement() != NULL && !mouseMiddlePressed)
	{
		MapElement *selectedElement = mapEditor->GetSelectedElement();
		if (!selectedElement->IsMoveActive() && !selectedElement->IsRotationActive() && !selectedElement->IsResizeActive())
		{
			mapEditor->SetSelectedElement(nullptr);
			mapEditor->SetSelectedElementChanged(true);
			mapEditor->SetMapElementsPropertiesChanged(true);
		}
	}

	if (mapEditor->GetAddBuilding() || mapEditor->GetAddDecoration() || mapEditor->GetAddParkPlace() || mapEditor->GetAddCar())
	{
		mapEditor->AddObstacleConfirm();
	}
}

void MapEditorGLHost::mouseReleaseEvent(QMouseEvent * event)
{
	OpenGLHost::mouseReleaseEvent(event);

	if (mapEditor->GetSelectedElement() != NULL)
	{
		MapElement *selectedElement = mapEditor->GetSelectedElement();
		if (selectedElement->IsAdmissible())
		{
			selectedElement->EnableMove(false);
			selectedElement->EnableRotation(false);
			selectedElement->EnableResize(false);
		}
	}
}

void MapEditorGLHost::mouseMoveEvent(QMouseEvent * event)
{
	OpenGLHost::mouseMoveEvent(event);

	positionOnMap = (glm::vec2(mouseLastX, mouseLastY) - drawAreaPosition - widgetOffset) / magnificationRatio;
	MapElement *selectedElement = mapEditor->GetSelectedElement();

	if (selectedElement != NULL)
	{
		int resizeCorner;

		selectedElement->SetMoveHover(false);
		selectedElement->SetRotationHover(false);
		selectedElement->SetResizeHover(false);

		if (mapEditor->GetMapElementToMove(positionOnMap) == selectedElement || selectedElement->IsMoveActive())
		{
			this->setCursor(MOVE_CURSOR);
			selectedElement->SetMoveHover(true);
			if (selectedElement->IsMoveActive())
			{
				selectedElement->Move(glm::vec2(mouseOffsetX, mouseOffsetY) / magnificationRatio);
				mapEditor->SetMapElementsPropertiesChanged(true);
			}
		}
		else if (mapEditor->GetMapElementToRotate(positionOnMap) == selectedElement || selectedElement->IsRotationActive())
		{
			this->setCursor(ROTATE_CURSOR);
			selectedElement->SetRotationHover(true);
			if (selectedElement->IsRotationActive())
			{
				if (positionOnMap.x > selectedElement->GetPosition().x)
				{
					if(positionOnMap.y > selectedElement->GetPosition().y)
						selectedElement->Rotate((-mouseOffsetX + mouseOffsetY) / 50.0f);
					else
						selectedElement->Rotate((mouseOffsetX + mouseOffsetY) / 50.0f);
				}
				else
				{
					if (positionOnMap.y > selectedElement->GetPosition().y)
						selectedElement->Rotate((-mouseOffsetX - mouseOffsetY) / 50.0f);
					else
						selectedElement->Rotate((mouseOffsetX - mouseOffsetY) / 50.0f);
				}
				mapEditor->SetMapElementsPropertiesChanged(true);
			}
		}
		else if (mapEditor->GetMapElementToResize(positionOnMap, &resizeCorner, selectedElement) == selectedElement || selectedElement->IsResizeActive())
		{
			this->setCursor(Qt::PointingHandCursor);
			selectedElement->SetResizeHover(true);
			if (!selectedElement->IsResizeActive())
				selectedElement->SetResizeHoverCorner(resizeCorner);
			if (selectedElement->IsResizeActive())
			{
				selectedElement->Resize(positionOnMap, selectedElement->GetResizeHoverCorner());
				mapEditor->SetMapElementsPropertiesChanged(true);
			}
		}
	}

	if (mapEditor->GetNewElement() == nullptr && !mapEditor->GetAddTerrain() && mapEditor->GetHoverElement(positionOnMap) != NULL && mapEditor->GetHoverElement(positionOnMap) != selectedElement)
	{
		if (selectedElement == NULL ||
			(!selectedElement->IsMoveHover() && !selectedElement->IsRotationHover() && !selectedElement->IsResizeHover()))
				this->setCursor(HOVER_CURSOR);
	}
	else
	{
		if (selectedElement != NULL)
		{
			if ((!selectedElement->IsMoveHover() && !selectedElement->IsRotationHover() && !selectedElement->IsResizeHover()))
				this->setCursor(DEFAULT_CURSOR);
		}
		else
			this->setCursor(DEFAULT_CURSOR);
	}

	if (mapEditor->GetAddTerrain() && mapEditor->GetNewTerrain() != nullptr && mouseLeftPressed)
	{
		Terrain *terrain = mapEditor->GetNewTerrain();
		terrain->SetPosition(positionOnMap);
		mapEditor->GetMap()->SetTerrainSlice(new Terrain(terrain->GetModelPath(), terrain->GetColor(), terrain->GetPosition()), positionOnMap.x, positionOnMap.y);
	}
	
	if (mapEditor->GetNewElement() != nullptr)
	{
		mapEditor->GetNewElement()->SetPosition(positionOnMap);
	}
}

void MapEditorGLHost::wheelEvent(QWheelEvent * event)
{
	OpenGLHost::wheelEvent(event);

	adjustMaxOffset();
}

void MapEditorGLHost::keyPressEvent(QKeyEvent * event)
{
	OpenGLHost::keyPressEvent(event);

	if (event->key() == Qt::Key_Delete)
	{
		if (mapEditor->GetSelectedElement() != NULL)
		{
			mapEditor->GetMap()->RemoveMapElement(mapEditor->GetSelectedElement());
			mapEditor->SetSelectedElement(nullptr);
			mapEditor->SetMapElementsChanged(true);
		}
	}
}

void MapEditorGLHost::keyReleaseEvent(QKeyEvent * event)
{
	OpenGLHost::keyReleaseEvent(event);
}

void MapEditorGLHost::SetMapEditor(MapEditor * mapEditor)
{
	this->mapEditor = mapEditor;
}

MapEditor * MapEditorGLHost::GetMapEditor()
{
	return this->mapEditor;
}

#pragma endregion

#pragma region OpenGL methods.

void MapEditorGLHost::initializeGL()
{
	OpenGLHost::initializeGL();
}

void MapEditorGLHost::resizeGL(int w, int h)
{
	OpenGLHost::resizeGL(w, h);

	adjustMaxOffset();
}

void MapEditorGLHost::paintGL()
{
	OpenGLHost::paintGL();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	nvgBeginFrame(vg, widgetWidth, widgetHeight, pixelRatio);

	nvgRenderFrame();

	nvgEndFrame(vg);
}

#pragma endregion

#pragma region Private methods.

void MapEditorGLHost::nvgRenderFrame()
{
	if (mapEditor->GetMap() == NULL) return;

	nvgHelper->DrawMap(mapEditor->GetMap());

	if (mapEditor->GetAddTerrain())
	{
		nvgHelper->DrawTerrain(mapEditor->GetHoverTerrain(positionOnMap), true);
	}

	if((mapEditor->GetNewElement() == nullptr && !mapEditor->GetAddTerrain() && mapEditor->GetSelectedElement() == NULL) ||
		(mapEditor->GetNewElement() == nullptr && !mapEditor->GetAddTerrain() && !mapEditor->GetSelectedElement()->IsMoveHover() && !mapEditor->GetSelectedElement()->IsRotationHover() && !mapEditor->GetSelectedElement()->IsResizeHover()))
			nvgHelper->DrawHoverElement(mapEditor->GetHoverElement(positionOnMap));

	nvgHelper->DrawSelectedElement(mapEditor->GetSelectedElement());

	if (mapEditor->GetNewElement() != nullptr)
	{
		bool admissible = mapEditor->IsMapElementAdmissible(mapEditor->GetNewElement());
		nvgHelper->DrawActiveElement(mapEditor->GetNewElement(), admissible);
	}
}

#pragma endregion