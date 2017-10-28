#include "CommonHelper.h"



CommonHelper::CommonHelper()
{
}


CommonHelper::~CommonHelper()
{
}

double CommonHelper::ConvertPixelsToMeters(double pixels)
{
	return pixels / ONE_METER;
}

double CommonHelper::ConverMetersToPixeks(double meters)
{
	return meters * ONE_METER;
}
