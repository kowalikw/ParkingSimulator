#ifndef COMMON_HELPER_H
#define COMMON_HELPER_H

#define ONE_METER 25
#define M_PI 3.14159265358979323846

class CommonHelper
{
public:
	static double ConvertPixelsToMeters(double pixels);
	static double ConverMetersToPixeks(double meters);
	static double ConvertRadiansToDegrees(double radians);
	static double ConvertDegreesToRadians(double degrees);
};

#endif // !COMMON_HELPER_H