#include "BSpline.h"
#include "GeometryHelper.h"
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream> 

#include <windows.h>

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


	//int h = 0;
	//int s = 0;
	//int k = 0;

	//vector<vector<glm::vec2>> a(n + 1, vector<glm::vec2>(m));
	//vector<vector<glm::vec2>> P(n + 1, vector<glm::vec2>(m));

	//if (u < knots[n] || u > knots[m - n])
	//	return glm::vec2();

	//for (int j = 0; j < m; j++)
	//{
	//	if ((u >= knots[j] && u < knots[j + 1]) || knots[j] == 1)
	//	{
	//		k = j;
	//		h = p;
	//		s = 0;

	//		break;
	//	}
	//}

	//for (int i = 0; i < controlPoints.size(); i++)
	//	P[i][0] = controlPoints[i];

	////for (int r = 1; r <= h; r++)
	//for(int r = 1; r <= n; r++)
	//{
	//	for (int i = k - n + r; i <= k; i++)
	//	{
	//		a[i][r].x = (u - knots[i]) / (knots[i + n - r + 1] - knots[i]);
	//		P[i][r].x = (1 - a[i][r].x) * P[i - 1][r - 1].x + a[i][r].x * P[i][r - 1].x;

	//		a[i][r].y = (u - knots[i]) / (knots[i + n - r + 1] - knots[i]);
	//		P[i][r].y = (1 - a[i][r].y) * P[i - 1][r - 1].y + a[i][r].y * P[i][r - 1].y;

	//		/*a[i][r].z = (u - knots[i]) / (knots[i + p - r + 1] - knots[i]);
	//		P[i][r].z = (1 - a[i][r].z) * P[i - 1][r - 1].z + a[i][r].z * P[i][r - 1].z;*/
	//	}
	//}

	////return P[k - s][p - s];
	//return P[k][n];
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

	
	//	return -999;

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

	//if (t < knots[n] || t > knots[m - n])
	//	return -999;

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
	double u = knots[n] + t * (knots[m - n] - knots[n]);

	/*std::ostringstream ss;
	ss << "t: " << t << endl;
	ss << "u: " << u << endl;
	ss << endl;
	std::string s(ss.str());

	OutputDebugStringA(s.c_str());*/

	SimulationState simulationState;
	simulationState.position = GetPoint(u);
	simulationState.angle = M_PI - GetAngle(u);

	return simulationState;
}
