#include "BSpline.h"

BSpline::BSpline()
{
}

BSpline::BSpline(vector<glm::vec2> controlPoints, int p)
{
	this->p = p;
	this->n = 3;
	this->m = n + controlPoints.size();
	this->controlPoints = controlPoints;
	this->knots = vector<double>(m + 1);

	for (int i = 0; i < m + 1; i++)
		knots[i] = i * 1.0 / m;
	/*knots[0] = knots[1] = knots[2] = knots[3] = 0;
	knots[4] = knots[5] = knots[6] = knots[7] = 1;*/
}

glm::vec2 BSpline::CalculatePoint(double u)
{
	int h = 0;
	int s = 0;
	int k = 0;

	vector<vector<glm::vec2>> a(n + 1, vector<glm::vec2>(m));
	vector<vector<glm::vec2>> P(n + 1, vector<glm::vec2>(m));

	if (u < knots[n] || u > knots[m - n])
		return glm::vec2();

	for (int j = 0; j < m; j++)
	{
		if ((u >= knots[j] && u < knots[j + 1]) || knots[j] == 1)
		{
			k = j;
			h = p;
			s = 0;

			break;
		}
	}

	for (int i = 0; i < controlPoints.size(); i++)
		P[i][0] = controlPoints[i];

	//for (int r = 1; r <= h; r++)
	for(int r = 1; r <= n; r++)
	{
		for (int i = k - n + r; i <= k; i++)
		{
			a[i][r].x = (u - knots[i]) / (knots[i + n - r + 1] - knots[i]);
			P[i][r].x = (1 - a[i][r].x) * P[i - 1][r - 1].x + a[i][r].x * P[i][r - 1].x;

			a[i][r].y = (u - knots[i]) / (knots[i + n - r + 1] - knots[i]);
			P[i][r].y = (1 - a[i][r].y) * P[i - 1][r - 1].y + a[i][r].y * P[i][r - 1].y;

			/*a[i][r].z = (u - knots[i]) / (knots[i + p - r + 1] - knots[i]);
			P[i][r].z = (1 - a[i][r].z) * P[i - 1][r - 1].z + a[i][r].z * P[i][r - 1].z;*/
		}
	}

	//return P[k - s][p - s];
	return P[k][n];
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
	return 0.0;
}

double BSpline::GetAngle(double t)
{
	return 0.0;
}

glm::vec2 BSpline::GetPoint(double t)
{
	return CalculatePoint(t);
}

SimulationState BSpline::GetSimulationState(double t)
{
	return SimulationState();
}
