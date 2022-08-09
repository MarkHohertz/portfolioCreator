#include "frontier.h"

frontier::frontier()
{
	//I tried making a constructor to call some of the methods but the timing of when data is available during runtime compicates things
}

frontier::~frontier()
{

}

void frontier::addStock(std::string s, double p, std::map<std::string, double> r, double avg, double std)  //adds stock and adjusts relevant portions of frontier object to reflect addition
{
	stock* creation = new stock(s, p, r, avg, std);    
	stockHolder.emplace_back(creation);
	Eigen::VectorXd temp(stockReturns.rows());
	temp = stockReturns;
	stockReturns.resize(stockHolder.size());
	stockReturns.segment(0, stockHolder.size() - 1) = temp;
	stockReturns(stockHolder.size() - 1) = pow((1+stockHolder.back()->dailyAVGRet),252)-1;  //place individual retuns into vector in annualized form
	numAssets = stockHolder.size();
	weights.resize(4, stockHolder.size());
}

void frontier::deleteStock(std::string ticker)		//removes stock and adjusts relevant portions of frontier object to reflect removal
{
	auto iterator = stockHolder.begin();
	
	for(int i=0; i<stockHolder.size(); i++)
	{
		if (stockHolder.at(i)->ticker == ticker)
		{
			stockHolder.erase(iterator);

			if (i == 0)
			{
				stockReturns = stockReturns.segment(1, stockHolder.size());
			}
			else
			{
				Eigen::VectorXd temp1 = stockReturns.segment(0, i);
				Eigen::VectorXd temp2 = stockReturns.segment(i + 1, stockReturns.rows()-i-1);
				stockReturns.resize(stockHolder.size());
				stockReturns.segment(0, i) = temp1;
				stockReturns.segment(i, stockReturns.rows()-i-1) = temp2;
			}
			
		}
		numAssets--;
		iterator++;
	}
	
}

double frontier::getSTD(portType type)
{
	return stds[type];
}

void frontier::setRiskFree(double rfl, double b)
{
	riskFree = rfl;
	borrow = b;
}

void frontier::calcReturn(double std, portType type)        //doesn't work for maxShp because you start off with weights not std for that particular portfolio     
{
	if (type == 0)
	{
		E = eBar;
	}
	else if (D * C * (pow(std, 2) - 1 / C) >= 0)
	{
		E = eBar + 1 / C * sqrt(D * C * (pow(std, 2) - 1 / C));
	}
	else if (risk == 0)
	{
		E = eBar;
	}
	
	portReturns[type] = E;
}

double frontier::getReturn(portType type)
{
	return portReturns[type];
}

void frontier::setRisk(double r)
{
	risk = r;
}

void frontier::setCovarMatrix()    //access underlying stock returns & calc variance covariance matrix
{
	double sum = 0;
	double divisor = 0;
	covariances.resize(stockHolder.size(),stockHolder.size());
	covariances.setZero();

	for (int i = 0; i < stockHolder.size(); i++)
	{
		for (int j = 0; j < stockHolder.size(); j++)
		{
			if (i == j)
			{
				covariances(i, j) = pow(stockHolder.at(i)->dailySTD, 2) * 252;        //adjusted to convert variances to yearly
			}
			else if (covariances(j,i) != 0)
			{
				covariances(i, j) = covariances(j, i);
			}
			else
			{
				
				auto iteratorR = stockHolder.at(i)->returns.begin(); 
				auto iteratorC = stockHolder.at(j)->returns.begin(); 
				
				for(int k=0; k<stockHolder.at(j)->returns.size(); k++) 
				{
					sum += ((iteratorR->second - stockHolder.at(i)->dailyAVGRet)*(iteratorC->second - stockHolder.at(j)->dailyAVGRet));
					iteratorR++;
					iteratorC++;
				}
				
				divisor = stockHolder.at(j)->returns.size()-1;
				covariances(i, j) = sum / divisor * 252;          //252 to convert covariances to yearly
				sum = 0;
			}
		}
	}
}

void frontier::calcWeights(portType type)  //must already know the return to get the weights using this method. cannot use for maxShp 
{
	
	firstSigma.resize(numAssets);
	secondSigma.resize(numAssets);
	
	for (int i = 0; i < numAssets; i++)
	{
		weights.resize(4,numAssets);
		firstSigma.fill(C);
		firstSigma = (firstSigma * stockReturns.array() - A) * inverseCVM.col(i).array();
		secondSigma.fill(-A);
		secondSigma = (secondSigma * stockReturns.array() + B) * inverseCVM.col(i).array();
		if (D != 0)
		{
			if ((abs(portReturns[type] * firstSigma.sum() + secondSigma.sum()) / D) < 0.0000000000001)    //using this to flush out extremely small weights
			{
				weights(type, i) = 0;
			}
			else
			{
				weights(type, i) =  (portReturns[type] * firstSigma.sum() + secondSigma.sum()) / D;
			}
		}
		else
		{
			weights(type, i) = 1;
		}
	}
}


Eigen::RowVectorXd frontier::getWeights(portType type)
{
	return weights.row(type);
}


double frontier::getStockReturn(int index)
{
	return stockReturns(index);
}

void frontier::calcIntermediateVars()         // calcs alphabetic variables mentioned in "An Analytic Derivation of the Efficient Frontier", based on various combos of covariances & returns
{
	inverseCVM = covariances.inverse();
	Amat = inverseCVM * stockReturns.asDiagonal();
	A = Amat.sum();
	Bmat = Amat.transpose();
	Bmat = Bmat * stockReturns.asDiagonal();
	Bmat.transposeInPlace();                        //if you want to broadcast rows multaplicatively, must 1)transpose array 2) multiply by vector 3) transpose the result
	B = Bmat.sum();
	C = inverseCVM.sum();
	if (abs(B * C - A * A) < 0.0000000000001)
	{
		D = 0;
	}
	else
	{
		D = B * C - A * A;
	}
	eBar = A / C;                  // eBar is the return on the min std portfolio on the efficient frontier
	portReturns[minVar] = eBar;
	stds[minVar] = sqrt(1 / C);        //stdBar - the minimum std portfolio on the efficient frontier
}

double frontier::getIntermediateVar(std::string var)
{
	if (var == "A")
	{
		return A;
	}
	else if (var == "B")
	{
		return B;
	}
	else if (var == "C")
	{
		return C;
	}
	else if (var == "D")
	{
		return D;
	}
	else if (var == "E")
	{
		return E;
	}
	else if (var == "eBar")
	{
		return eBar;
	}
}

void frontier::calcMaxSTD() //this does not calculate the true maxSTD. I do not know of a way to analytically solve for this. Enhancement may require random portfolio generation.
{
	double max = 0;
	double hIndex = 0;
	Eigen::VectorXd weightHolder;
	weightHolder.resize(stockReturns.rows());

	for (int i = 0; i < stockReturns.rows(); i++)
	{
		if (stockHolder.at(i)->dailySTD > max || i == 0)
		{
			max = stockHolder.at(i)->dailySTD;
			hIndex = i;
		}
	}
	 
	stds[maxSTD] = stockHolder.at(hIndex)->dailySTD * sqrt(252);
	
}

void frontier::calcMaxSharpeStdAndWeights()       // weights -> portfolio return -> std
{
	Eigen::ArrayXd mxShpHldr;
	Eigen::ArrayXd varHldr;
	Eigen::ArrayXd weightedRets;
	mxShpHldr.resize(stockReturns.rows());
	varHldr.resize(stockReturns.rows());
	weightedRets.resize(stockReturns.rows());

	for (int i = 0; i < stockReturns.rows(); i++)
	{
		varHldr = inverseCVM.col(i).array() * (stockReturns.array() - riskFree);
		mxShpHldr(i) = varHldr.sum()/ (A - riskFree * C);           
	}
	for (int i = 0; i < stockReturns.rows(); i++)
	{
		weights(maxShp, i) = mxShpHldr(i);
	}
	
	varhldrtest.resize(varHldr.rows());
	maxshrphldrtest.resize(mxShpHldr.rows());
	varhldrtest = varHldr;
	maxshrphldrtest = mxShpHldr;

	weightedRets = mxShpHldr.array() * stockReturns.array();
	portReturns[maxShp] = weightedRets.sum();
	if (D == 0)
	{
		stds[maxShp] = stockHolder.at(0)->dailySTD* sqrt(252);
	}
	else
	{
		stds[maxShp] = sqrt((C*pow(portReturns[maxShp], 2) - 2 * A * portReturns[maxShp] + B) / D); //sqrt(pow(stds[0], 2) + pow((portReturns[2] - portReturns[0]), 2) / (D * C));         //added c here to see if it works
	}
}

void frontier::calcRiskPrefSTD()    //use user risk pref to calc preferred std
{
	stds[riskPref] = (stds[maxSTD] - stds[minVar]) * risk + stds[minVar];
}