#include <cstdlib>
#include <climits> 
#include <ctime>
#include <iterator>
#include <qdebug.h>
#include <random>
#include <qstring.h>
#include "normal.h"

Normal::Normal(unsigned int seed, int n) :
	n(n)
	/*A({ {0, 3.38713287279637E+00},
		{1, 1.33141667891784E+02},
		{2, 1.97159095030655E+03},
		{3, 1.37316937655095E+04},
		{4, 4.59219539315499E+04},
		{5, 6.72657709270087E+04},
		{6, 3.34305755835880E+04},
		{7, 2.50908092873012E+03}}),
	B({ {0, 1.00000000000000E+00}, 
		{1, 4.23133307016009E+01}, 
		{2, 6.87187007492058E+02}, 
		{3, 5.39419602142475E+03}, 
		{4, 2.12137943015866E+04}, 
		{5, 3.93078958000927E+04}, 
		{6, 2.87290857357219E+04}, 
		{7, 5.22649527885285E+03} }),
	C({ {0, 1.42343711074968E+00},
		{1, 4.63033784615654E+00},
		{2, 5.76949722146069E+00},
		{3, 3.64784832476320E+00},
		{4, 1.27045825245237E+00},
		{5, 2.41780725177451E-01},
		{6, 2.27238449892692E-02},
		{7, 7.74545014278341E-04} }),
	D({ {0, 1.00000000000000E+00},
		{1, 2.05319162663776E+00},
		{2, 1.67638483018380E+00},
		{3, 6.89767334985100E-01},
		{4, 1.48103976427480E-01},
		{5, 1.51986665636165E-02},
		{6, 5.47593808499534E-04},
		{7, 1.05075007164442E-09} }),
	E({ {0, 6.65790464350110E+00},
		{1, 5.46378491116411E+00},
		{2, 1.78482653991729E+00},
		{3, 2.96560571828504E-01},
		{4, 2.65321895265761E-02},
		{5, 1.24266094738807E-03},
		{6, 2.71155556874348E-05},
		{7, 2.01033439929228E-07} }),
	F({ {0, 1.00000000000000E+00},
		{1, 5.99832206555887E-01},
		{2, 1.36929880922735E-01},
		{3, 1.48753612908506E-02},
		{4, 7.86869131145613E-04},
		{5, 1.84631831751005E-05},
		{6, 1.42151175831644E-07},
		{7, 2.04426310338993E-15} })*/

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

double Normal::marsagliaPolarNext(double mean, double stdDev)
{
	static double spare;
	static bool hasSpare = false;

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

/*double Normal::wichuraNext()
{
	double uniDistVal = (double)rand() / (double)RAND_MAX;
	double Kn = 0.0, R = 0.0, q = uniDistVal - 0.5;

	if ((q >= -1E-15) && (q <= 1E-15))
	{
		Kn = uniDistVal;
	}
	else if ((q > -0.425) && (q < 0.425))
	{
		R = pow(0.425, 2) - pow(q, 2);

		double up = 0.0, down = 0.0;

		for (std::map<short int, double>::reverse_iterator it = A.rbegin(); it != A.rend(); ++it)
		{
			up = up * R + it->second;
		}
		for (std::map<short int, double>::reverse_iterator it = B.rbegin(); it != B.rend(); ++it)
		{
			down = down * R + it->second;
		}

		Kn = up / down;

		Kn = (((((((A[7] * R + A[6]) * R + A[5]) * R + A[4]) * R + A[3]) * R + A[2]) * R + A[1]) * R + A[0]) / 
			(((((((B[7] * R + B[6]) * R + B[5]) * R + B[4]) * R + B[3]) * R + B[2]) * R + B[1]) * R + B[0]);
	}
	else if (q <= -0.425)
	{
		R = sqrt(-log(uniDistVal));
	}
	else if (q >= 0.425)
	{
		R = sqrt(-log(1 - uniDistVal));
	}

	if (R == 5)
	{
		R = R - 1.6;

		double up = 0.0, down = 0.0;

		for (std::map<short int, double>::reverse_iterator it = C.rbegin(); it != C.rend(); ++it)
		{
			up = up * R + it->second;
		}
		for (std::map<short int, double>::reverse_iterator it = D.rbegin(); it != D.rend(); ++it)
		{
			down = down * R + it->second;
		}

		Kn = up / down;

		Kn = (((((((C[7] * R + C[6]) * R + C[5]) * R + C[4]) * R + C[3]) * R + C[2]) * R + C[1]) * R + C[0]) /
			(((((((D[7] * R + D[6]) * R + D[5]) * R + D[4]) * R + D[3]) * R + D[2]) * R + D[1]) * R + D[0]);
	}
	else if (R > 5)
	{
		R = R - 5.0;

		double up = 0.0, down = 0.0;

		for (std::map<short int, double>::reverse_iterator it = E.rbegin(); it != E.rend(); ++it)
		{
			up = up * R + it->second;
		}
		for (std::map<short int, double>::reverse_iterator it = F.rbegin(); it != F.rend(); ++it)
		{
			down = down * R + it->second;
		}

		Kn = up / down;

		Kn = (((((((E[7] * R + E[6]) * R + E[5]) * R + E[4]) * R + E[3]) * R + E[2]) * R + E[1]) * R + E[0]) /
			(((((((F[7] * R + F[6]) * R + F[5]) * R + F[4]) * R + F[3]) * R + F[2]) * R + F[1]) * R + F[0]);
	}

	return 0.5 + ((rand() / ((double)RAND_MAX)) * 2.0 - 1.0) * Kn * 0.15;
}*/