#include "Visualisation.h"

#include "PathPlanner.h"

Visualisation::Visualisation()
{
	this->visualisation2D = true;
	this->visualisation3D = false;
}

bool Visualisation::GetVisualisation2D()
{
	return this->visualisation2D;
}

bool Visualisation::GetVisualisation3D()
{
	return this->visualisation3D;
}

void Visualisation::SetVisualisation2D(bool visualisation2D)
{
	this->visualisation2D = visualisation2D;
}

void Visualisation::SetVisualisation3D(bool visualisation3D)
{
	this->visualisation3D = visualisation3D;
}

bool Visualisation::GetShowPath()
{
	return this->showPath;
}

void Visualisation::SetShowPath(bool showPath)
{
	this->showPath = showPath;
}

void Visualisation::SetCurrentSimulation(Simulation * simulation)
{
	this->currentSimulation = simulation;

	this->visualisationChanged = true;
}

Simulation * Visualisation::GetCurrentSimulation()
{
	return this->currentSimulation;
}

std::vector<Simulation*> Visualisation::GetSimulations()
{
	return this->simulations;
}

void Visualisation::AddSimulation(Simulation * simulation)
{
	this->simulations.push_back(simulation);
}

void Visualisation::RemoveSimulation(Simulation * simulation)
{
	std::vector<Simulation*> newSimulations;
	for (int i = 0; i < simulations.size(); i++)
		if (simulations[i] != simulation)
			newSimulations.push_back(simulations[i]);
	this->simulations = newSimulations;
}

bool Visualisation::GetVisualisationChanged()
{
	return this->visualisationChanged;
}

void Visualisation::SetVisualisationChanged(bool visualisationChanged)
{
	this->visualisationChanged = visualisationChanged;
}
