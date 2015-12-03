/*****************************************************************************************************************************
Author: Peter Forgacs <peter.forgacs3@gmail.com>
Date: 2015
Description: FDR Custom class that handles random samples and calculates their p value which gets stored in the result global.
******************************************************************************************************************************/
#pragma once
//Includes
#include "GSEA.h"
#include "Random_Sample.h"
#include <unordered_set>
#include <string>
#include <mutex>
#include <gsl/gsl_cdf.h>


//Global variables
extern std::map<std::string, std::unordered_set<std::string>> population_success;
extern std::map<std::string, std::pair<double, std::vector<double>>> result;
extern std::mutex m;

class FDR 
{
private:
	//----Data members----//
	int sample_size;
	int obj_rep_num;
	int pop_size;
	vector<unordered_set<string>> r_samples;
	unordered_set<string> r_sample;

	
	//----Methods----//
	//----Make Methods----//
	bool make_r_sample_result();
		std::map<std::string, int> get_sample_success();
		double p_hyper(unsigned int k, unsigned int n1, unsigned int n2, unsigned int t);

public:
	//----Methods----//
	//----Constructors - Sets up the objects data members ----//
	FDR(R_sample & obj);
	//----Controller method - Controls the flow of the object, calls the make methods
	bool simulation_controller();
};

