#include "BezierCurve.h"

BezierCurve::BezierCurve()
{
}

BezierCurve::BezierCurve(std::vector<glm::vec2> coefficients)
{
	this->coefficients = coefficients;
}


BezierCurve::~BezierCurve()
{
}

double BezierCurve::GetLength()
{
	return 0.0;
}

double BezierCurve::GetAngle(double t)
{
	return 0.0;
}

glm::vec2 BezierCurve::GetFirstPoint()
{
	return glm::vec2();
}

glm::vec2 BezierCurve::GetLastPoint()
{
	return glm::vec2();
}

glm::vec2 BezierCurve::GetPoint(double t)
{
	return DeCasteljau(t);
}

SimulationState BezierCurve::GetSimulationState(double t)
{
	return SimulationState();
}

glm::vec2 BezierCurve::DeCasteljau(double t)
{
	int n = coefficients.size();
	double u = 1 - t;
	std::vector<std::vector<glm::vec2>> b;
	for (int i = 0; i < n; i++)
	{
		b.push_back(std::vector<glm::vec2>());
		for (int j = 0; j < n; j++)
			b[i].push_back(glm::vec2());
	}
	for (int i = 0; i < n; i++)
		b[i][0] = coefficients[i];

	for (int j = 1; j < n; j++)
		for (int i = 0; i < n - j; i++)
		{
			b[i][j].x = b[i][j - 1].x * u + b[i + 1][j - 1].x * t;
			b[i][j].y = b[i][j - 1].y * u + b[i + 1][j - 1].y * t;
		}

	return n == 0 ? glm::vec2(-1, -1) : b[0][n - 1];
}
