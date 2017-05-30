#include "PathPlanner.h"

PathPlanner::PathPlanner() { }

void PathPlanner::AddUserPoint(glm::vec2 userPoint)
{
	this->userPoints.push_back(userPoint);
}

void PathPlanner::SetUserPoints(std::vector<glm::vec2> userPoints)
{
	this->userPoints = userPoints;
}

std::vector<glm::vec2> PathPlanner::UserPoints()
{
	return this->userPoints;
}

std::vector<glm::vec2> PathPlanner::VoronoiPoints()
{
	return this->voronoiPoints;
}
