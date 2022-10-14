#include "stock.h"

stock::stock(std::string s)
{
	ticker = s;
}

stock::stock(std::string s, double p, std::map<std::string, double> r, double avg, double std)
{
	ticker = s;
	setPrice(p);
	setPriceData();
	calcReturnMap();
	calcDailyAVGRet();
	calcDailySTD();
	calcYearlySTD();
	calcBeta(r, avg, std);
}

stock::~stock()
{

}

void stock::calcReturnMap()
{
	auto j = prices.begin();
	auto i = prices.begin();
	i++;
	for (i; i != prices.end(); i++)
	{
		returns.emplace(i->first, (i->second - j->second)/j->second);
		j++;
	}
}

void stock::calcDailySTD()
{
	double add = 0;
	for (auto i = returns.begin(); i != returns.end(); i++)
	{
		add += pow(i->second - dailyAVGRet, 2);
	}
	dailySTD = sqrt(add / (returns.size() - 1));
}

void stock::calcYearlySTD()
{
	yearlySTD = dailySTD * sqrt(252);
}

void stock::calcDailyAVGRet()
{
	std::ofstream myfile;
	myfile.open("Data/" + ticker + "Returns.txt");
	double product = 0;
	double temp = 0;

	for (auto i = returns.begin(); i != returns.end(); i++)
	{
		if (std::distance(returns.begin(),i) == 0)
		{
			temp = 1;
		}
		else
		{
			temp = product;
		}
		product = (1.0 + i->second) * temp;
		myfile << i->first << ", " << i->second << "\n";
	}
	
	dailyAVGRet = pow(product, 1.0/returns.size())-1.0;
}


void stock::setPriceData()   //requestor class generates .txt files that need to be put into data structure to more easily manipulate
{
	std::ifstream fileReader;
	std::string row;
	std::string date;
	std::string price;
	std::stringstream ss;

	fileReader.open("Data/" + ticker + ".txt");

	if (fileReader.is_open())
	{
		while (fileReader.good())
		{
			fileReader >> row;
			std::replace(row.begin(), row.end(), ',', ' ');
			std::stringstream ss(row);
			ss >> date;
			ss >> price;
			ss >> price;
			ss >> price;
			ss >> price;
			ss >> price;
			if (date != "Close" && date != "Date")
			{
				prices.emplace(date, std::stod(price));
			}
		}
	}
}

void stock::calcBeta(std::map<std::string, double> indexReturns, double indexAVGDailyRet, double indexSTD)
{
	
	auto i = returns.begin();
	auto j = indexReturns.begin();   
	double add = 0;

	for (i; i != returns.end(); i++)
	{
		add += (i->second-dailyAVGRet)*(j->second-indexAVGDailyRet); 
		j++;
	}

	double covar = add / (returns.size() - 1);

	beta = covar / pow(indexSTD,2);
}

void stock::setPrice(double p)
{
	curPrice = p;
}

void stock::setIndustry(std::string i)
{
	industry = i;
}
