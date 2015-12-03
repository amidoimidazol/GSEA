/******************************************************************************************************
Author: Peter Forgacs <peter.forgacs3@gmail.com>
Date: 2015
Description:The R_sample class is a container class. 
			It gets used by the Thread Controller and FDR classes to create and store
			the random samples that are needed for the FDR test.
			It uses Fisher Yates shuffle to create random samples from the original population.
			The size of the r_sample container depends on the number of threads and repetition number.
*******************************************************************************************************/
#pragma once
#include <vector>
#include <unordered_set>

using namespace std;

class R_sample
{
public:
	//----Methods----//
	//----Constructor: Auto calls random sample creation----//
	R_sample(int sample_size_, int obj_rep_num_, std::vector<std::string> & pop_);
	
	//----Data members----//
	int sample_size;
	int obj_rep_num;
	vector<unordered_set<string>> r_samples;
	int pop_size;

private:
	//----Data members----//
	std::vector<std::string> pop;
	vector<string> backup_pop;

	//----Methods----//
	void make_r_samples();
		template<class bidiiter> bidiiter random_unique(bidiiter begin, bidiiter end, size_t num_random);
};
