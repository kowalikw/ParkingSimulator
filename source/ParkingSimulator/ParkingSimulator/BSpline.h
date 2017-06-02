#ifndef B_SPLINE_H
#define B_SPLINE_H

#include <vector>
#include "PathElement.h"

using namespace std;

class BSpline : public PathElement
{
public:
	BSpline();
	BSpline(vector<glm::vec2> controlPoints, int p = 3);

	glm::vec2 CalculatePoint(double u);
private:
	int n; // n+1 control points
	int m; // m+1 knots
	int p; // degree of b-spline

	vector<glm::vec2> controlPoints;
	vector<double> knots;
};

#endif