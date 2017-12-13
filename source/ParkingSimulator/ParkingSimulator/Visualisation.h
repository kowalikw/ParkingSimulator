#ifndef VISUALISATION_H
#define VISUALISATION_H

#include <vector>
#include "PathPlanner.h"
#include "Simulation.h"
#include "Graph.h"

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

	bool GetVisualisationChanged();
	void SetVisualisationChanged(bool visualisationChanged);
private:
	bool showPath = false;
	bool visualisation2D = false;
	bool visualisation3D = false;
	bool visualisationChanged = false;
	Simulation *currentSimulation = NULL;
	std::vector<Simulation*> simulations;
};

#endif