#ifndef PATH_PLANNER_H
#define PATH_PLANNER_H

#include <vector>
#include <glm.hpp>

class PathPlanner
{
public:
	PathPlanner();

	void AddUserPoint(glm::vec2 userPoint);
	void SetUserPoints(std::vector<glm::vec2> userPoints);

	std::vector<glm::vec2> UserPoints();
	std::vector<glm::vec2> VoronoiPoints();
private:
	std::vector<glm::vec2> userPoints;
	std::vector<glm::vec2> voronoiPoints;
};

#endif