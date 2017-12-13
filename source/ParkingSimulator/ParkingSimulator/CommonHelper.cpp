#include "CommonHelper.h"

double CommonHelper::ConvertPixelsToMeters(double pixels)
{
	return pixels / ONE_METER;
}

double CommonHelper::ConverMetersToPixeks(double meters)
{
	return meters * ONE_METER;
}

double CommonHelper::ConvertRadiansToDegrees(double radians)
{
	return radians * 180.0 / M_PI;
}

double CommonHelper::ConvertDegreesToRadians(double degrees)
{
	return degrees * M_PI / 180.0;
}