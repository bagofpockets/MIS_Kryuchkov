#include <cstdlib>
#include <climits> 
#include <ctime>
#include "simpson.h"

Simpson::Simpson(unsigned int seed, const double lowerBound, const double upperBound, const double modeL, const double modeR) :
	lowerBound(lowerBound),
	upperBound(upperBound),
	modeL(modeL),
	modeR(modeR),
	hight(2 / ((upperBound - lowerBound) + (modeR - modeL)))
{
	std::srand(seed);
}

Simpson::~Simpson()
{

}

double Simpson::next()
{
	double uniDistVal = (double)rand() / (double)RAND_MAX;

	if (uniDistVal <= (hight / 2 * (modeL - lowerBound)))
	{
		return lowerBound + sqrt(uniDistVal * 2 * (modeL - lowerBound) / hight);
	}
	else if (uniDistVal <= 1 - (hight / 2 * (upperBound - modeR)))
	{
		return uniDistVal / hight + (lowerBound + modeL) / 2;
	}
	else
	{
		return upperBound - sqrt((1 - uniDistVal) * 2 * (upperBound - modeR) / hight);
	}

}