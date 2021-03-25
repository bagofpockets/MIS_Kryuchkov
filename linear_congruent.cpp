#include "linear_congruent.h"

LinearCongruent::LinearCongruent(unsigned int seed, const unsigned int samples, const unsigned int mult, const unsigned int increment) :
	samples(samples),
	mult(mult),
	increment(increment),
	seed(seed)
{

}

LinearCongruent::~LinearCongruent()
{

}

void LinearCongruent::setSeed(unsigned int val)
{
	seed = val;
}

double LinearCongruent::next()
{
	seed = (mult * seed + increment) % samples;
	return double(seed) / double(samples);
}