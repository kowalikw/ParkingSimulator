#include "BSpline.h"

BSpline::BSpline()
{
}

BSpline::BSpline(vector<glm::vec2> controlPoints, int p)
{
	std::vector<glm::vec2> newControlPoints;
	for (int i = 0; i < controlPoints.size(); i++)
	{
		if (i == 0 || i == controlPoints.size() - 1)
		{
			newControlPoints.push_back(controlPoints[i]);
			newControlPoints.push_back(controlPoints[i]);
			newControlPoints.push_back(controlPoints[i]);
		}
		else
		{
			newControlPoints.push_back(controlPoints[i]);
		}
	}

	this->p = p;
	this->n = 3;
	this->m = n + newControlPoints.size();
	this->controlPoints = newControlPoints;
	this->knots = vector<double>(m + 1);

	for (int i = 0; i < m + 1; i++)
		knots[i] = i * 1.0 / m;
	
	// calculate length
	glm::vec2 currentPoint, prevPoint;
	for (double t = 0; t < 1.0; t += 0.01)
	{
		if (t == 0)
			prevPoint = GetPoint(t);
		else
		{
			prevPoint = currentPoint;
			currentPoint = GetPoint(t);
			this->length += GeometryHelper::GetDistanceBetweenPoints(prevPoint, currentPoint);
		}
	}
}

glm::vec2 BSpline::CalculatePoint(double t)
{
	int l = 0;
	for (int j = 0; j < m; j++)
	{
		if ((t >= knots[j] && t < knots[j + 1]) || knots[j] == 1)
		{
			l = j;
			break;
		}
	}

	vector<vector<float>>a(n + 1, vector<float>(m));
	for (int k = 1; k <= n; k++)
	{
		for (int i = l - n + k; i <= l; i++)
		{
			a[k][i] = (t - knots[i]) / (knots[i + n + 1 - k] - knots[i]);
		}
	}

	vector<vector<glm::vec2>> d(n + 1, vector<glm::vec2>(m));

	for (int i = 0; i < controlPoints.size(); i++)
		d[0][i] = controlPoints[i];

	for (int k = 1; k <= n; k++)
	{
		for (int i = l - n + k; i <= l; i++)
		{
			d[k][i] = (1 - a[k][i]) * d[k - 1][i - 1] + a[k][i] * d[k - 1][i];
		}
	}

	return d[n][l];
}

double BSpline::GetPointX(double t)
{
	int l = 0;
	for (int j = 0; j < m; j++)
	{
		if ((t >= knots[j] && t < knots[j + 1]) || knots[j] == 1)
		{
			l = j;
			break;
		}
	}

	vector<vector<double>> a(n + 1, vector<double>(m));
	for (int k = 1; k <= n; k++)
	{
		for (int i = l - n + k; i <= l; i++)
		{
			a[k][i] = (t - knots[i]) / (knots[i + n + 1 - k] - knots[i]);
		}
	}

	vector<vector<double>> d(n + 1, vector<double>(m));

	for (int i = 0; i < controlPoints.size(); i++)
		d[0][i] = controlPoints[i].x;

	for (int k = 1; k <= n; k++)
	{
		for (int i = l - n + k; i <= l; i++)
		{
			d[k][i] = (1 - a[k][i]) * d[k - 1][i - 1] + a[k][i] * d[k - 1][i];
		}
	}

	return d[n][l];
}

double BSpline::GetPointY(double t)
{
	int l = 0;
	for (int j = 0; j < m; j++)
	{
		if ((t >= knots[j] && t < knots[j + 1]) || knots[j] == 1)
		{
			l = j;
			break;
		}
	}

	vector<vector<double>> a(n + 1, vector<double>(m));
	for (int k = 1; k <= n; k++)
	{
		for (int i = l - n + k; i <= l; i++)
		{
			a[k][i] = (t - knots[i]) / (knots[i + n + 1 - k] - knots[i]);
		}
	}

	vector<vector<double>> d(n + 1, vector<double>(m));

	for (int i = 0; i < controlPoints.size(); i++)
		d[0][i] = controlPoints[i].y;

	for (int k = 1; k <= n; k++)
	{
		for (int i = l - n + k; i <= l; i++)
		{
			d[k][i] = (1 - a[k][i]) * d[k - 1][i - 1] + a[k][i] * d[k - 1][i];
		}
	}

	return d[n][l];
}

std::vector<glm::vec2> BSpline::GetControlPoints()
{
	return this->controlPoints;
}

double BSpline::GetLength()
{
	if (this->length == 0)
	{
		glm::vec2 currentPoint, prevPoint;
		for (double t = 0; t < 1.0; t += 0.01)
		{
			if (t == 0)
				prevPoint = GetPoint(t);
			else
			{
				prevPoint = currentPoint;
				currentPoint = GetPoint(t);
				this->length += GeometryHelper::GetDistanceBetweenPoints(prevPoint, currentPoint);
			}
		}
	}
	return this->length;
}

double BSpline::GetAngle(double t)
{
	glm::vec2 dir = GeometryHelper::GetLineDirection(GetPoint(t), GetPoint(t - 0.001));

	return atan2(dir.y, dir.x);
}

double BSpline::GetInsideAngle(double t, double wheelbase, double track)
{
	auto turnRadius = 1.0 / (2 * GetCurvature(t));
	double angle = atan(wheelbase / (turnRadius - track / 2.0f));
	if (GetDirection(t) == Right)
		angle *= -1;
	return angle;
}

double BSpline::GetCurvature(double t)
{
	if (t >= 0.01 && t <= 0.99f)
	{
		auto d1 = GetPoint(t - 0.01f) - GetPoint(t);
		auto d2 = GetPoint(t) - GetPoint(t + 0.01f);

		double angle = GeometryHelper::GetAngleBetweenVectors(d1, d2);
		double length = 0;
		for (double u = t - 0.01 + 0.002; u < t + 0.01 + 0.002; u += 0.002)
		{
			length += GeometryHelper::GetDistanceBetweenPoints(GetPoint(u - 0.002), GetPoint(u));
		}

		return abs(angle / length);
	}

	return 0;
}

glm::vec2 BSpline::GetFirstPoint()
{
	return CalculatePoint(knots[n]);
}

glm::vec2 BSpline::GetLastPoint()
{
	return CalculatePoint(knots[m - n]);
}

glm::vec2 BSpline::GetPoint(double t)
{
	if(t < knots[n]) return CalculatePoint(knots[n]);
	if(t > knots[m - n]) return CalculatePoint(knots[m - n]);

	return CalculatePoint(t);
}

SimulationState BSpline::GetSimulationState(double t)
{
	if (t < 0.1) t = 0.1f; // clamp min
	if (t > 0.9f) t = 0.9f; // clamp max

	double u = knots[n] + t * (knots[m - n] - knots[n]);

	SimulationState simulationState;
	simulationState.position = GetPoint(u);
	simulationState.angle = M_PI - GetAngle(u);
	simulationState.curvature = GetCurvature(u);
	simulationState.direction = GetDirection(u);

	return simulationState;
}

CircleType BSpline::GetDirection(double t)
{
	if (t >= 0.01 && t <= 0.99f)
	{
		if (GeometryHelper::GetVectorsDirection(GetPoint(t - 0.001f), GetPoint(t), GetPoint(t + 0.001f)) == CircleType::Left)
			return CircleType::Right;
		return CircleType::Left;
	}
	return CircleType::Undefined;
}
