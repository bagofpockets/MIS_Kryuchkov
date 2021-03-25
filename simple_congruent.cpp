#include <QDebug>
#include "simple_congruent.h"

SimpleCongruent::SimpleCongruent(unsigned int seed, const unsigned int samples, const unsigned int mult) :
	samples(samples),
	mult(mult),
	seed(seed)
{

}

SimpleCongruent::~SimpleCongruent()
{

}

void SimpleCongruent::setSeed(unsigned int val)
{
	seed = val;
}

double SimpleCongruent::next()
{
	seed = (mult * seed) % samples;
	return double(seed) / double(samples);
}