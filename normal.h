#pragma once
#include <map>
#include <string>
#define TWO_PI 6.2831853071795864769252866

class Normal
{
public:
	Normal(unsigned int seed = 1, const int n = 12);
	~Normal();

	double standartNext();
	//double wichuraNext();
	double marsagliaPolarNext(double mean = 0.5, double stdDev = 0.08);

private:
	const int n;
	/*std::map<short int, double> A;
	std::map<short int, double> B;
	std::map<short int, double> C;
	std::map<short int, double> D;
	std::map<short int, double> E;
	std::map<short int, double> F;*/
};