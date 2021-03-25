#pragma once
#include <climits> 
#include <ctime>

#define SAMPLES UINT_MAX
#define MULT 22695477
#define INCREMENT 23

class LinearCongruent
{
public:
	LinearCongruent(unsigned int seed = 1, const unsigned int samples = SAMPLES, const unsigned int mult = MULT, const unsigned int increment = INCREMENT);
	~LinearCongruent();

	void setSeed(unsigned int val = std::time(0));
	double next();

private:
	const unsigned int samples, mult, increment;
	unsigned int seed;
};