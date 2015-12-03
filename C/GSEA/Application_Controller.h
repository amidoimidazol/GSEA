/******************************************************************************************
Author: Peter Forgacs <peter.forgacs3@gmail.com>
Date: 2015
Description:	The Application_Controller class controls the main workflow of the program.
				It will handle input / multiple files / MPI in the future.
				This is the class that gets called by main.cpp to start the test.
*******************************************************************************************/
#pragma once
// STL
#include <string>
#include <sstream>
#include <set>
#include <algorithm>
#include <fstream>
#include <time.h>
// STL Random
#include  <random>
#include  <iterator>
// STL Multithread
#include <thread>
// GSL
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>
// TCLAP
//#include <tclap/CmdLine.h>
// Custom Classes
#include "config.h"
#include "Thread_Controller.h"
#include "GSEA.h"		
#include "Random_Sample.h"
#include "FDR.h"

class Application_Controller
{
public:
	//----Methods----//
	//----Constructor: Default constructor----//
	Application_Controller(){};
	// Controls the workflow
	void new_test();
		// Launches GSEA test
		GSEA run_gsea();
		// Launches FDR test trough Thread_Controller
		void run_fdr(GSEA test_);
};

inline void display_result() {
	/*
	Displays the results from the result container.
	Writes them to a file
	*/

	ofstream myfile("./Log/result.txt");
	if (myfile.is_open())
	{
		for (auto &elem : result) {
			myfile << "Category name: " << elem.first << " - ";
			myfile << "Category P value: " << elem.second.first << "\n";
			myfile << "RNG Matrix " << elem.first << ":" << "\n";
			for (auto &elem2 : elem.second.second) {
				myfile << elem2 << " ";
			}
			myfile << "\n";
		}
		myfile.close();
	}
}
