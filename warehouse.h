#pragma once
#include <vector>
#include "qvector.h"

class Warehouse
{
public:
	Warehouse(std::vector<int> bottomLine, std::vector<int> delivery, const unsigned int days, const unsigned int tovars = 6);
	~Warehouse();

	std::pair<double, std::vector<std::pair<QVector<double>, QVector<double>>>> run_model(unsigned int ver);

private:
	std::vector<int> bottomLine, delivery;
	std::vector<std::vector<int>> maxDemand;
	const int days, tovars;
};