#pragma once
#include <climits> 
#include <ctime>

#define SAMPLES UINT_MAX
#define MULT 22695477
#define INCREMENT 23

class Uniform
{
public:
	Uniform(unsigned int seed = 1, const unsigned int samples = SAMPLES, const unsigned int mult = MULT, const unsigned int increment = INCREMENT);
	~Uniform();

	double standardNext();
	double simpleCongruentNext();
	double linearCongruentNext();

private:
	const unsigned int samples, mult, increment;
	unsigned int seed;
};