#pragma once

class Exponential
{
public:
	Exponential(unsigned int seed = 1, const int intervals = 100, double lambda = 1.5, const int k = 1);
	~Exponential();

	double standardNext();
	double hyperNext();
	double erlangNext();

private:
	double lambda;
	const int intervals, k;
};