#pragma once

#include "cpr/cpr.h"
#include "wx/wx.h"
#include <vector>
#include <iostream>
#include <cpr/cpr.h>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>

class requester
{
public:
	requester();
	~requester();
	void updateStock(std::string stock);
	void pullData(int horizon);
	std::string givePrice();
	std::string giveIndustry();
	void pullRiskFree();
	double getRiskFree();
	std::string getResult();
	double riskFree;

private:
	std::string stockTicker = "";
	std::string price;
	std::string industry;
	std::string dataPull;
};
