#include "requester.h"

requester::requester()
{
	
}

requester::~requester()
{

}

void requester::updateStock(std::string stock)
{
	this->stockTicker = stock;
}

void requester::pullData(int horizon)       //assembles URL based on user input, makes HTTP request, saves data from request
{
	
	std::stringstream urlHistorical;
	

	//query fields
	std::string period1;
	std::string period2;
	std::string interval;
	std::string events;

	//a duration has a count of periods and a ratio describing the number of seconds that elapses in each period	
	typedef std::chrono::duration<int> sec_type;

	//a time point has a clock and a duration
	std::chrono::time_point<std::chrono::system_clock, sec_type> today = std::chrono::time_point_cast<sec_type>(std::chrono::system_clock::now());
	int today1 = today.time_since_epoch().count();
	std::chrono::seconds secs(horizon * 31556952);
	int prev = today1 - secs.count();

	//period 1 needs to change based off how long the investing horizon (# of years) is for the user. Output needs to be number of seconds since (-before) 1970 Jan 1
	period1 = std::to_string(prev);
	period2 = std::to_string(today1);
	interval = "1d";
	events = "history";


	urlHistorical << "https://query1.finance.yahoo.com/v7/finance/download/" << stockTicker << "?period1=" << period1 << "&period2=" << period2 << "&interval=" << interval << "&events=history&AdjustedClose=true";
	auto r = cpr::Get(cpr::Url{ urlHistorical.str() });
	dataPull = r.text;

	if (dataPull.substr(0, 3) == "404" || dataPull.substr(0, 3) == "400")
	{
		//Do nothing if an error is returned
	}
	else 
	{
		std::ofstream fileWriter;
		fileWriter.open("Data/" + stockTicker + ".txt");
		fileWriter << dataPull;
		fileWriter.close();
	}

}

std::string requester::givePrice()          //current price for stockList on page 2
{
	auto mktinsider = cpr::Get(cpr::Url{ "https://markets.businessinsider.com/stocks/" + stockTicker +"-stock" });
	std::string dataPullmktinsider = mktinsider.text;
	if (dataPullmktinsider.find("Sorry, the page you were looking for could not be found") == -1)
	{
		std::string findPrice = "price-section__current-value";
		int priceIndex = dataPullmktinsider.find(findPrice);
		int priQuoteIndex = dataPullmktinsider.find("</span>", priceIndex) - (priceIndex + 30);
		price = dataPullmktinsider.substr(priceIndex + 30, priQuoteIndex);
	}
	else
	{
		price = "-1";
	}
	return price;
}

std::string requester::giveIndustry()         //industry for stockList on page 2
{	
	auto yahoo = cpr::Get(cpr::Url{ "https://finance.yahoo.com/quote/" + stockTicker + "/profile?p=AAL" });
	std::string dataPullYahoo = yahoo.text;
	if (dataPullYahoo.substr(0, 3) == "404" || dataPullYahoo.substr(0,3) == "400")
	{
		industry = "N/A";
	}
	else
	{
		std::string findIndustry = "<span>Industry</span>";
		int industryIndex = dataPullYahoo.find(findIndustry);
		int indBracketIndex = dataPullYahoo.find("</span>", industryIndex + 46) - (industryIndex + 46);
		industry = dataPullYahoo.substr(industryIndex + 46, indBracketIndex);
		if (industry.find("&amp;") != -1) {
			industry.replace(industry.find("&amp;"), 5, "&");
		}
		else if (industry.find("—") != -1) {
			industry.replace(industry.find("—"), 3, "-");
		}
	}
	
	return industry;
}


void requester::pullRiskFree()          //risk free rate - important part of many finance related calculations
{
	auto cnbc = cpr::Get(cpr::Url{ "https://www.cnbc.com/quotes/?symbol=US10Y" });
	std::string dataPullcnbc = cnbc.text;
	if (dataPullcnbc.find("Page not found") == -1)
	{
		std::string findRate = "<span class=\"QuoteStrip-lastPrice\">";
		int rfIndex = dataPullcnbc.find(findRate);
		int rfPercIndex = dataPullcnbc.find("</span>", rfIndex + 34) - (rfIndex + 36);
		riskFree = std::stod(dataPullcnbc.substr(rfIndex + 35, rfPercIndex));
	}
	else
	{
		riskFree = 0;
	}
}

double requester::getRiskFree()
{
	return riskFree;
}

std::string requester::getResult()
{
	return dataPull;
}