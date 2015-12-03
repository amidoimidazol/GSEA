/**************************************************************************************************************
Author: Peter Forgacs <peter.forgacs3@gmail.com>
Date: 2015
Description:	GSEA custom class reads in given files and performs hypergeometric test on the given datasets.
				The results are stored in the result global.
***************************************************************************************************************/
#pragma once
// Includes
#include <string>
#include <map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>
#include <time.h>
#include <iterator>  // back inserter
// Custom Classes
#include "E.h"

// Global variables
extern std::map<std::string, std::unordered_set<std::string>> population_success;
extern std::map<std::string, std::pair<double, std::vector<double>>> result;

class GSEA
{
public:
	//----Data members----//
	// Vector version of population that is needed for shuffle randomization in FDR
	std::vector<std::string> v_population;
	int sample_size;

	//----Methods----//
	//----Constructors: Constructor controls the flow of the object, calls the make methods----//
	GSEA(const char* database_file, const char* population_file, const char* sample_file);

private:
	//----Data members----//
	// Input file variables gets set by the default constructor
	const char* database_file;
	const char* population_file;
	const char* sample_file;

	// Container and size variables
	std::unordered_set<std::string> population;
	std::unordered_set<std::string> sample;

	//----Make methods - Control the sequence of the algorithm----//
	//----Misc methods - get called by the appropriate make method----//
	bool make_population();
	void make_db_pop();
	// Database intersect pool
	void find_common_genes(const std::string& category_name, const std::unordered_set<std::string>& genes_in_category);
	bool make_sample();
	bool make_result(std::unordered_set<std::string>& sample);
	// Pool success intersect sample
	std::map<std::string, int> get_sample_success(std::unordered_set<std::string> & sample);
	double p_hyper(unsigned int k, unsigned int n1, unsigned int n2, unsigned int t);

	//----General Methods----//
	std::unordered_set<std::string> read_file(const char* file);

	//----Check methods----//
	void check_sample();
	void check_population();

	//----Display methods----
	void display_set(std::unordered_set<std::string> & set);
	void display_common(std::map<std::string, std::unordered_set<std::string>> & common);
};

