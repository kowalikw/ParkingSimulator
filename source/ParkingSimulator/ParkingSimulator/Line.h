#ifndef LINE_H
#define LINE_H

#include "PathElement.h"

class Line : public PathElement
{
public:
	Line(glm::vec2 from, glm::vec2 to, ManeuverType maneuverType = ManeuverType::Front);
	glm::vec2 GetFrom() const;
	glm::vec2 GetTo() const;
	void SetFrom(glm::vec2 from);
	void SetTo(glm::vec2 to);

	virtual double GetLength();
	virtual double GetAngle(double t);
	virtual glm::vec2 GetPoint(double t);
	virtual SimulationState GetSimulationState(double t);
//private:
	glm::vec2 from;
	glm::vec2 to;
};

#endif