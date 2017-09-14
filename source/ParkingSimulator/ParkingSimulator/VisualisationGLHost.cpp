#include "VisualisationGLHost.h"

VisualisationGLHost::VisualisationGLHost(QWidget *parent) : OpenGLHost(parent) 
{
	simulationTimer = new QTimer(this);
	simulationTimer->setInterval(10);
	connect(simulationTimer, SIGNAL(timeout()), this, SLOT(simulationTimerTick()));

	simulationTimer->start();
}

#pragma region User input events.

void VisualisationGLHost::mousePressEvent(QMouseEvent * event)
{
	OpenGLHost::mousePressEvent(event);
}

void VisualisationGLHost::mouseReleaseEvent(QMouseEvent * event)
{
	OpenGLHost::mouseReleaseEvent(event);
}

void VisualisationGLHost::mouseMoveEvent(QMouseEvent * event)
{
	OpenGLHost::mouseMoveEvent(event);
}

void VisualisationGLHost::keyPressEvent(QKeyEvent * event)
{
	OpenGLHost::keyPressEvent(event);
}

void VisualisationGLHost::keyReleaseEvent(QKeyEvent * event)
{
	OpenGLHost::keyReleaseEvent(event);
}

void VisualisationGLHost::SetVisualisation(Visualisation * visualisation)
{
	this->visualisation = visualisation;
}

Visualisation * VisualisationGLHost::GetVisualisation()
{
	return this->visualisation;
}

#pragma endregion

#pragma region OpenGL methods.

void VisualisationGLHost::initializeGL()
{
	OpenGLHost::initializeGL();
}

void VisualisationGLHost::resizeGL(int w, int h)
{
	OpenGLHost::resizeGL(w, h);
}

void VisualisationGLHost::paintGL()
{
	OpenGLHost::paintGL();

	if (visualisation == NULL) return;

	glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	nvgBeginFrame(vg, widgetWidth, widgetHeight, pixelRatio);

	nvgRenderFrame();
	
	nvgEndFrame(vg);
}

#pragma endregion

#pragma region Private methods.

void VisualisationGLHost::nvgRenderFrame()
{
	if (visualisation->GetCurrentSimulation() != NULL)
	{
		nvgHelper->DrawSimulationFrame(visualisation->GetCurrentSimulation());

		//nvgHelper->DrawGraph(&visualisation->voronoi);

		bool showPath = visualisation->GetShowPath();
		if (visualisation->GetShowPath())
			nvgHelper->DrawPath(visualisation->GetCurrentSimulation()->GetPath());
	}
	auto mapElements = visualisation->mapa2->GetMapElements();
	auto vehicle = visualisation->GetCurrentSimulation()->GetVehicle();
	for (int i = 0; i < mapElements.size(); i++)
	{
		auto polygon = GeometryHelper::ExpandPolygon(mapElements[i]->GetPoints(), 3.0f * vehicle->track / 4.0f);

		nvgHelper->DrawPolygon(polygon);
	}
}

#pragma endregion

#pragma region Slots.

void VisualisationGLHost::simulationTimerTick()
{
	if (visualisation->GetCurrentSimulation() != NULL)
	{
		Simulation *currentSimulation = visualisation->GetCurrentSimulation();
		
		if(currentSimulation->IsStarted())
			currentSimulation->Step();
	}
}

#pragma endregion