#pragma once
#include <climits> 
#include <ctime>

#define SAMPLES UINT_MAX
#define MULT 22695477

class SimpleCongruent
{
public:
	SimpleCongruent(unsigned int seed = 1, const unsigned int samples = SAMPLES, const unsigned int mult = MULT);
	~SimpleCongruent();

	void setSeed();
	double next();

private:
	const unsigned int samples, mult;
	unsigned int seed;
};