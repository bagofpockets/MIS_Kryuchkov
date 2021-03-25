#pragma once

class Simpson
{
public:
	Simpson(unsigned int seed = 1, const double lowerBound = 0.0, const double upperBound = 1.0, const double modeL = 0.35, const double modeR = 0.65);
	~Simpson();

	double next();

private:
	const double lowerBound;
	const double upperBound;
	const double modeL;
	const double modeR;
	const double hight;
};
