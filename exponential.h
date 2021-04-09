#pragma once

class Exponential
{
public:
	Exponential(unsigned int seed = 1, const int intervals = 100, double lambda = 1.5);
	~Exponential();

	double standardNext();
	double hyperNext();

private:
	double lambda;
	const int intervals;
};