#include <cstdlib>
#include "uniform.h"

Uniform::Uniform(unsigned int seed, const unsigned int samples, const unsigned int mult, const unsigned int increment) :
	samples(samples),
	mult(mult),
	increment(increment),
	seed(seed)
{
	std::srand(seed);
}

Uniform::~Uniform()
{

}

double Uniform::standardNext()
{
	return (double)rand() / (double)RAND_MAX;
}

double Uniform::simpleCongruentNext()
{
	seed = (mult * seed) % samples;
	return double(seed) / double(samples);
}

double Uniform::linearCongruentNext()
{
	seed = (mult * seed + increment) % samples;
	return double(seed) / double(samples);
}