#include "linear_congruent.h"

LinearCongruent::LinearCongruent(unsigned int seed = 1, const unsigned int samples, const unsigned int mult, const unsigned int increment) :
	samples(samples),
	mult(mult),
	increment(increment),
	seed(seed)
{

}

LinearCongruent::~LinearCongruent()
{

}

void LinearCongruent::setSeed()
{
	seed = std::time(0);
}

double LinearCongruent::next()
{
	seed = (mult * seed + increment) % samples;
	return double(seed) / double(samples);
}