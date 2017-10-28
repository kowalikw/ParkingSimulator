#ifndef COMMON_HELPER_H
#define COMMON_HELPER_H

#define ONE_METER 25

class CommonHelper
{
public:
	CommonHelper();
	~CommonHelper();

	static double ConvertPixelsToMeters(double pixels);
	static double ConverMetersToPixeks(double meters);
};

#endif // !COMMON_HELPER_H