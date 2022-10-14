#pragma once
#include "stock.h"
#include <vector>
#include <Eigen/Dense>
#include <map>
#include <cmath>
#include <time.h>

enum portType { minVar, maxSTD, maxShp, riskPref };

class frontier
{
public:

	frontier();
	~frontier();
	void addStock(std::string s, double p, std::map<std::string, double> r, double avg, double std);  //1 connection to stock
	void deleteStock(std::string ticker);         
	double getSTD(portType type);
	void setRiskFree(double rfl, double b);
	void calcReturn(double std, portType type);
	double getReturn(portType type);
	void setRisk(double r);           //input from frame
	void setCovarMatrix();
	void calcWeights(portType type);
	Eigen::RowVectorXd getWeights(portType type);
	double getStockReturn(int index);
	void calcIntermediateVars();
	double getIntermediateVar(std::string var);
	void calcMaxSTD();
	void calcMaxSharpeStdAndWeights();
	void calcRiskPrefSTD();
	Eigen::MatrixXd testWeights;
	Eigen::ArrayXd sums;
	Eigen::ArrayXXd weightArray; //temp placing here for testing purposes
	Eigen::VectorXd stockReturns; //expected returns of the individual stocks	TEMPORARILY MOVED HERE
	std::vector<stock*> stockHolder; //temp putting this here to make testing easier
	Eigen::MatrixXd covariances;
	Eigen::MatrixXd inverseCVM;
	int count;
	double betas[4];
	double riskFree = 0;

private:
	
	
	
	double stds[4];                 // 0 = stdBar, the min variance port, 1 = maximum std efficient portfolio, 2= max sharpe ratio port, 3= risk preference std
	double portReturns[4];
	double risk;
	double leverage;
	double size;
	double borrow;
	
	Eigen::MatrixXd frontierPoints;
	Eigen::MatrixXd weights;
	std::vector<double> targetPortfolio;   //weights for the portfolio that aligns most with investor's risk tolerance
	std::vector<double> min_var_alloc;	   //weights for the safest portfolio
	std::vector<double> max_sharpe_alloc;  //weights for the most efficient portfolio
	int numAssets;                     //number of securities
	
	
	Eigen::MatrixXd Amat;       //These are matrices that contain values used in the calculation of intermediate values 
	Eigen::MatrixXd Bmat;		//Labelled analagously to those in Robert C. Merton's paper - "An Analytic Derivation of the Efficient Portfolio"
	Eigen::MatrixXd Cmat;
	Eigen::MatrixXd Emat;
	double A, B, C, D, eBar;    //intermediate values calculated from the lettered matrices above 
	double E;					//return on efficient portfolio at a given standard deviation
	Eigen::ArrayXd firstSigma;   //vector for use in calculating the stock weights
	Eigen::ArrayXd secondSigma;  //vector for use in calculating the stock weights

};
