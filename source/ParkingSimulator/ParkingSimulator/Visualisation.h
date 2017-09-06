#ifndef VISUALISATION_H
#define VISUALISATION_H

#include <vector>
#include "Simulation.h"

class Visualisation
{
public:
	Visualisation();

	bool GetVisualisation2D();
	bool GetVisualisation3D();
	void SetVisualisation2D(bool visualisation2D);
	void SetVisualisation3D(bool visualisation3D);
	bool GetShowPath();
	void SetShowPath(bool showPath);
	void SetCurrentSimulation(Simulation *simulation);
	Simulation *GetCurrentSimulation();
	std::vector<Simulation*> GetSimulations();
	void AddSimulation(Simulation *simulation);
	void RemoveSimulation(Simulation *simulation);
private:
	bool showPath = false;
	bool visualisation2D = false;
	bool visualisation3D = false;
	Simulation *currentSimulation;
	std::vector<Simulation*> simulations;
};

#endif