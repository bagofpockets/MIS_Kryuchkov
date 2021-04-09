#include <cstdlib>
#include "exponential.h"

Exponential::Exponential(unsigned int seed, const int intervals, double lambda) :
	lambda(lambda),
	intervals(intervals)
{
	std::srand(seed);
}

Exponential::~Exponential()
{

}

double Exponential::standardNext()
{
	return -log((double)rand() / (double)RAND_MAX) / lambda;
}

double Exponential::hyperNext()
{
	int i = 1;
	double S = 1.0 / (double)intervals, q = S, uniDistModel = (double)rand() / (double)RAND_MAX;

	while (uniDistModel > S)
	{
		i++;
		S += q;
	}

	return -log((double)rand() / (double)RAND_MAX) / i;
}