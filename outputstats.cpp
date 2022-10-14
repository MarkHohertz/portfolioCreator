#include "outputstats.h"

outputstats::outputstats(Eigen::RowVectorXd w, std::vector<stock*> v, double inv)    //adjusting members to fit dataset
{
	weights.resize(v.size());
	weights = w;
	g2lweights = w;
	values.resize(v.size());
	invest = inv;
	g2lweights.resize(v.size());
	g2lLabels.resize(v.size());
	g2lvalues.resize(v.size());
	stockBetas.resize(v.size());
	for (int i = 0; i < v.size(); i++)
	{
		
		g2lLabels[i] = v[i]->ticker;
		stockBetas[i] = v[i]->beta;
	
	}
	CalcDollarAmts();
	greatestToLeast();
}

outputstats::~outputstats()
{

}

void outputstats::setLeverage(double l)
{
	//leverage = l;
}

double outputstats::getLeverage()
{
	//return leverage;
}

int outputstats::getNumAssets()
{
	//return numAssets;
}

void outputstats::calcPortBeta()
{
	product.resize(1);
	product	=  weights * stockBetas;
	portBeta = product.sum();
}

double outputstats::calcTreynor(double portRet, double riskFree, double beta)
{
	double treynor;
	treynor = (portRet - riskFree) / beta;
	return treynor;
}

double outputstats::calcSharpe(double portRet, double riskFree, double stDev)
{
	double sharpe;
	sharpe = (portRet - riskFree) / stDev;
	return sharpe;
}

double outputstats::calcSTD(double dailyStd)                   //need STD for S&P 500 & Russell 2000
{
	return dailyStd * 252;
}

double outputstats::CalcFV(double portRet, double size, int years)
{
	return size * pow((1 + portRet), years);
}

void outputstats::CalcDollarAmts()
{
	values = invest * g2lweights; 
	g2lvalues = values;
}

Eigen::RowVectorXd outputstats::GetDollarAmts()
{
	return g2lvalues;
}

std::vector<std::string> outputstats::GetTickers()
{
	return g2lLabels;
}

Eigen::RowVectorXd outputstats::getWeights()
{
	return g2lweights;
}

void outputstats::greatestToLeast()    //sort algo - made from scratch, almost certainly not the optimal solution
{
	bool reordered = true;
	double tempWeight = 0;
	double tempValue = 0;
	std::string tempTicker = "";
	while (reordered)
	{
		for (auto i = 0; i < g2lLabels.size()-1; i++)
		{
			reordered = false;
			if (g2lweights(i) < g2lweights(i+1))
			{
				tempWeight = g2lweights(i);
				tempTicker = g2lLabels[i];
				tempValue = g2lvalues[i];
				g2lweights(i) = g2lweights(i + 1);
				g2lLabels[i] = g2lLabels[i+1];
				g2lvalues[i] = g2lvalues[i + 1];
				g2lweights(i + 1) = tempWeight;
				g2lLabels[i+1] = tempTicker;
				g2lvalues[i + 1] = tempValue;
				reordered = true;
			}
			
		}
	}
	
}
