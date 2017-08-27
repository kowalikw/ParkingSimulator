#include "Visualisation.h"

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
}
