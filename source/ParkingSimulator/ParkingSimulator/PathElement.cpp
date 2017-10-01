#include "PathElement.h"

ManeuverType PathElement::GetManeuverType()
{
	return this->maneuverType;
}

void PathElement::SetManeuverType(ManeuverType maneuverType)
{
	this->maneuverType = maneuverType;
}

double PathElement::GetLengthToElement()
{
	return this->lengthToElement;
}

void PathElement::SetLengthToElement(double length)
{
	this->lengthToElement = length;
}
