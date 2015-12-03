/**************************************************************************************************************************
Author: Peter Forgacs <peter.forgacs3@gmail.com>
Date: 2015
Description: Hypergeometric is program that performs GSEA (Gene Set Enrichment Analysis) using the hypergeomteric formula.
			 After it generated data from GSEA performs an FDR(False Discovery Rate) test on the data previously generated. 
***************************************************************************************************************************/
// Dirty fix for g++ hypot_ compilation problem
#include <cmath>
//STL
#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
// STL Multithread
#include <mutex>
// Custom Classes
#include "GSEA.h"		
#include "Application_Controller.h"

// Namespace
using namespace std;

// Global variables
std::map<std::string, std::unordered_set<std::string>> population_success;
std::map<std::string, std::pair<double, std::vector<double>>> result;
std::mutex m;


int main(int argc, char* argv[])
{	
	try
	{	
		Application_Controller my_application;
		my_application.new_test();
		display_result();
	}
	catch (GSEA_EXCEPTION& ex) {
		/*
			Reports caught exceptions.
			Unopenable files.
			Population and sample not subsets.
			Bad repetition values.
			TODO Possibly log errors log_and_show(ex.what())
		*/
		cout << "Error! " <<endl;
		cout << ex.what() << ex.get_info() << " !" << endl;
		cout << "Function " << ex.get_func() << "()"<< endl;
		cout << "Location: " << ex.get_file() << " on line " << ex.get_line() << "." << endl;
		system("pause");
		return EXIT_FAILURE;
	}
	catch (exception& e)
	{                           
		/*
			Catch all standard exceptions
		*/
		cout << e.what() << endl;
		system("pause");
		return EXIT_FAILURE;
	}

	system("pause");
	return 0;
}



