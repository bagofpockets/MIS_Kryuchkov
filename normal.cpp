#include <cstdlib>
#include <climits> 
#include <ctime>
#include <iterator>
#include <qdebug.h>
#include "normal.h"

Normal::Normal(unsigned int seed, int n) :
	n(n)
{
	std::srand(seed);
}

Normal::~Normal()
{

}

double Normal::standartNext()
{
	double uniDistModel = 0.0;

	for (int i = 0; i < n; i++)
	{
		uniDistModel += (double)rand() / (double)RAND_MAX;
	}

	return uniDistModel / 12;
}

double Normal::marsagliaPolarNext()
{
	static double spare;
	static bool hasSpare = false;

	double mean = 0.5, stdDev = 0.08;

	if (hasSpare) {
		hasSpare = false;
		return spare * stdDev + mean;
	}
	else {
		double u, v, s;
		do {
			u = (rand() / ((double)RAND_MAX)) * 2.0 - 1.0;
			v = (rand() / ((double)RAND_MAX)) * 2.0 - 1.0;
			s = u * u + v * v;
		} while (s >= 1.0 || s == 0.0);
		s = sqrt(-2.0 * log(s) / s);
		spare = v * s;
		hasSpare = true;
		return mean + stdDev * u * s;
	}
}