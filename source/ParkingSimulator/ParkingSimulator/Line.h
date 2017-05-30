#ifndef LINE_H
#define LINE_H

#include "PathElement.h"

class Line : public PathElement
{
public:
	Line(glm::vec3 from, glm::vec3 to);
	glm::vec3 GetFrom() const;
	glm::vec3 GetTo() const;
private:
	glm::vec3 from;
	glm::vec3 to;
};

#endif