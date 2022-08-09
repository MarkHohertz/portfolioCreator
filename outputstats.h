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
		void calcTreynor();                //need treynor for both preferred portfolio & S&P 500
		double getTreynor();				
		void calcSharpe();					//need sharpe for both preferred portfolio and S&P 500
		//calcSTD()?                   need STD for S&P 500 & Russell 2000
		//Calc10Yrs()                 calculate value of preferred portfolio and its largest 3 holdings in 10 yrs.
		void CalcDollarAmts();			//using weights of preferred portfolio& initial investment size, calc $ amts for each stock
		Eigen::RowVectorXd GetDollarAmts();        //  Frame needs to be able to get the dollar amts from outputstats so it can fill in the list
		std::vector <std::string> GetTickers();
		Eigen::RowVectorXd getWeights();
		void greatestToLeast();

		std::vector<std::string> g2lLabels;

		Eigen::RowVectorXd weights;
		Eigen::RowVectorXd values;
		
		Eigen::RowVectorXd g2lweights;
		Eigen::RowVectorXd g2lvalues;

		double invest;
		
};
