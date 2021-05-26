#include <ctime>
#include "uniform.h"
#include "simpson.h"
#include "normal.h"
#include "warehouse.h"

Warehouse::Warehouse(std::vector<int> bottomLine, std::vector<int> delivery, const unsigned int days, const unsigned int tovars) :
	bottomLine(bottomLine),
	delivery(delivery),
	days(days),
	tovars(tovars),
	maxDemand({
		{0, 50, 34, 20, 9, 4},
		{190, 0, 55, 32, 15, 30},
		{270, 150, 0, 44, 20, 55},
		{350, 200, 97, 0, 25, 80},
		{430, 250, 118, 68, 0, 105},
		{510, 300, 139, 80, 35, 04}
		})
{

}

Warehouse::~Warehouse()
{

}

std::pair<double, std::vector<std::pair<QVector<double>, QVector<double>>>> Warehouse::run_model(unsigned int ver)
{
	std::vector<std::pair<QVector<double>, QVector<double>>> result;
	for (unsigned int tovar = 0; tovar < tovars; tovar++)
	{
		QVector<double> xData(days, 0.0), yData(days, 0.0);
		std::pair<QVector<double>, QVector<double>> xyData = std::make_pair(xData, yData);
		result.push_back(xyData);
	}

	std::vector<int> allProductRequest(6, 0), quantityMissingProducts(6, 0), succesProduct(6, 0), storage = { 2500, 1500, 1000, 500, 250, 150 };

	switch (ver)
	{
	case 1: //Равномерное
	{
		Uniform val(std::time(0));
		for (int day = 0; day < days; day++)
		{
			int currentEveryRequest[6][6];
			for (int consumer = 0; consumer < 6; consumer++)
			{
				for (int product = 0; product < 6; product++)
				{
					double req = val.linearCongruentNext() * (double)maxDemand[consumer][product];
					currentEveryRequest[consumer][product] = (int)req;
				}
			}

			std::vector<int> currentProductRequest(6, 0);
			for (int product = 0; product < 6; product++)
			{
				for (int consumer = 0; consumer < 6; consumer++)
				{
					currentProductRequest[product] += currentEveryRequest[consumer][product];
				}
				allProductRequest[product] += currentProductRequest[product];
				storage[product] -= currentProductRequest[product];
				
				if (storage[product] < 0)
				{
					quantityMissingProducts[product] += abs(storage[product]);
					succesProduct[product] = succesProduct[product] + storage[product] + currentProductRequest[product];
					storage[product] = delivery[product];
				}
				else
				{
					if (storage[product] < bottomLine[product])
					{
						storage[product] += delivery[product];
						succesProduct[product] = succesProduct[product] + currentProductRequest[product];
					}
					else
					{
						succesProduct[product] = succesProduct[product] + currentProductRequest[product];
					}
				}
				result[product].first[day] = day;
				result[product].second[day] = succesProduct[product];
			}
		}
		break;
	}
	case 2: //Треугольное
	{
		Simpson val(std::time(0), 0, 1, 0.5, 0.5);
		for (int day = 0; day < days; day++)
		{
			int currentEveryRequest[6][6];
			for (int consumer = 0; consumer < 6; consumer++)
			{
				for (int product = 0; product < 6; product++)
				{
					double req = val.next() * (double)maxDemand[consumer][product];
					currentEveryRequest[consumer][product] = (int)req;
				}
			}

			std::vector<int> currentProductRequest(6, 0);
			for (int product = 0; product < 6; product++)
			{
				for (int consumer = 0; consumer < 6; consumer++)
				{
					currentProductRequest[product] += currentEveryRequest[consumer][product];
				}
				allProductRequest[product] += currentProductRequest[product];
				storage[product] -= currentProductRequest[product];

				if (storage[product] < 0)
				{
					quantityMissingProducts[product] += abs(storage[product]);
					succesProduct[product] = succesProduct[product] + storage[product] + currentProductRequest[product];
					storage[product] = delivery[product];
				}
				else
				{
					if (storage[product] < bottomLine[product])
					{
						storage[product] += delivery[product];
						succesProduct[product] = succesProduct[product] + currentProductRequest[product];
					}
					else
					{
						succesProduct[product] = succesProduct[product] + currentProductRequest[product];
					}
				}
				result[product].first[day] = day;
				result[product].second[day] = succesProduct[product];
			}
		}
		break;
	}
	case 3: //Нормальное
	{
		Normal val(std::time(0));
		for (int day = 0; day < days; day++)
		{
			int currentEveryRequest[6][6];
			for (int consumer = 0; consumer < 6; consumer++)
			{
				for (int product = 0; product < 6; product++)
				{
					double req = val.marsagliaPolarNext() * (double)maxDemand[consumer][product];
					currentEveryRequest[consumer][product] = (int)req;
				}
			}

			std::vector<int> currentProductRequest(6, 0);
			for (int product = 0; product < 6; product++)
			{
				for (int consumer = 0; consumer < 6; consumer++)
				{
					currentProductRequest[product] += currentEveryRequest[consumer][product];
				}
				allProductRequest[product] += currentProductRequest[product];
				storage[product] -= currentProductRequest[product];

				if (storage[product] < 0)
				{
					quantityMissingProducts[product] += abs(storage[product]);
					succesProduct[product] = succesProduct[product] + storage[product] + currentProductRequest[product];
					storage[product] = delivery[product];
				}
				else
				{
					if (storage[product] < bottomLine[product])
					{
						storage[product] += delivery[product];
						succesProduct[product] = succesProduct[product] + currentProductRequest[product];
					}
					else
					{
						succesProduct[product] = succesProduct[product] + currentProductRequest[product];
					}
				}
				result[product].first[day] = day;
				result[product].second[day] = succesProduct[product];
			}
		}
		break;
	}
	default:
		break;
	}

	double allRequest = 0.0, allMissing = 0.0;

	for (size_t i = 0; i < 6; i++)
	{
		allRequest += allProductRequest[i];
		allMissing += quantityMissingProducts[i];
	}
	double percentCompleteRequest = 100 - (allMissing / allRequest) * 100;

	return  std::make_pair(percentCompleteRequest, result);
}