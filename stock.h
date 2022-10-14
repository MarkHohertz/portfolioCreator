#pragma once
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>


class stock
{
public:
	stock(std::string s);
	stock(std::string s, double p, std::map<std::string, double> r, double avg, double std);
	~stock();
	void calcReturnMap();
	void calcDailySTD();
	void calcYearlySTD();
	void calcDailyAVGRet();
	void setPriceData();
	void calcBeta(std::map<std::string, double> indexReturns, double indexAVGDailyRet, double indexSTD);
	void setPrice(double p);
	void setIndustry(std::string i);
	
	
	double beta;
	double curPrice;
	double dailyAVGRet;
	double dailySTD;
	double yearlySTD;
	std::string industry;
	std::map<std::string, double> returns;
	std::map<std::string, double> prices;
	std::string ticker;
	
};  
