#pragma once
#include "Eigen/Dense"
#include "frontier.h"
#include "vector"
#include "stock.h"

class outputstats {
	public:
		//
		outputstats(Eigen::RowVectorXd w, std::vector<stock*> v, double inv);
		~outputstats();
		void setLeverage(double l);         
		double getLeverage();				
		int getNumAssets();                 //could get this from statistics
		void calcPortBeta();
		double calcTreynor(double portRet, double riskFree, double beta);                //need treynor for both preferred portfolio & S&P 500
		double getTreynor();				
		double calcSharpe(double portRet, double riskFree, double stDev);					//need a way to calc sharpe for s&p 500
		double calcSTD(double dailyStd);                   //need STD for S&P 500 & Russell 2000
		double CalcFV(double portRet, double size, int years);
		void CalcDollarAmts();			//using weights of preferred portfolio& initial investment size, calc $ amts for each stock
		Eigen::RowVectorXd GetDollarAmts();        //  Frame needs to be able to get the dollar amts from outputstats so it can fill in the list
		std::vector <std::string> GetTickers();
		Eigen::RowVectorXd getWeights();
		void greatestToLeast();

		std::vector<std::string> g2lLabels;

		Eigen::RowVectorXd weights;
		Eigen::RowVectorXd values;
		Eigen::VectorXd stockBetas;
		Eigen::RowVectorXd g2lweights;
		Eigen::RowVectorXd g2lvalues;
		Eigen::VectorXd product;

		double invest;
		double portBeta;
};
