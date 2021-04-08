#pragma once
#include <map>

class Normal
{
public:
	Normal(unsigned int seed = 1, const int n = 12);
	~Normal();

	double standartNext();
	double marsagliaPolarNext();

private:
	const int n;
};