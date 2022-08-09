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
	for (int i = 0; i < v.size(); i++)
	{
		
		g2lLabels[i] = v[i]->ticker;
	
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
