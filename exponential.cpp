#include <cstdlib>
#include "exponential.h"

Exponential::Exponential(unsigned int seed, const int intervals, double lambda, const int k) :
	lambda(lambda),
	intervals(intervals),
	k(k)
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

	return -log((double)rand() / (double)RAND_MAX) / (lambda * i);
}

double Exponential::erlangNext()
{
	double nextVal = 0.0;

	for (int i = 0; i < k; i++)
	{
		nextVal += standardNext();
	}
	return nextVal;
}