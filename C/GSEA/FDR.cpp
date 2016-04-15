/*****************************************************************************************************************************
Author: Peter Forgacs <peter.forgacs3@gmail.com>
Date: 2015
Description: FDR Custom class that handles random samples and calculates their p value which gets stored in the result global.
******************************************************************************************************************************/
#include "FDR.h"
//----Constructor----//
FDR::FDR(R_sample& obj)
{
	r_samples = obj.r_samples;
	obj_rep_num = obj.obj_rep_num;
	sample_size = obj.sample_size;
	pop_size = obj.pop_size;
};

//----Controller----//
bool FDR::simulation_controller()
{	
	/*
		When called runs the make methods in sequence.
		The number of times it repeats depends on the number we initalizied the object with.
	*/
	for (long i = 0; i < obj_rep_num; ++i)
	{
		clock_t makrsamp = clock();

		for(auto i : r_samples)
		{	
			r_sample = i;
			make_r_sample_result();
		}

		//m2.lock();
		//ofstream myfile("./Log/thread_log.txt", std::ios_base::app);
		//if (myfile.is_open())
		//{
		//	myfile << "Thread id: " << this_thread::get_id() << endl;
		//	myfile << "Object id: " << this << endl;
		//	/*for (auto i : r_sample) {
		//		myfile << i << " ";
		//	}*/
		//	myfile << "\n";
		//}
		//myfile.close();
		//m2.unlock();

		printf("Make sample: %.2fs\n", (double)(clock() - makrsamp) / CLOCKS_PER_SEC);
	}
	return true;
}

//----Sample intersect Population success----//
bool FDR::make_r_sample_result() {
	/*
	Inserts the p value of a random sample into the result global variable. 
	Calculates the random sample intersect population success value.
	Generates a p value which get inserted

	sample_success			|	std::map<std::string, int>
	population_success		|	std::map<std::string, std::unordered_set<std::string>>
	result					|	std::map<std::string, std::pair<double, std::vector<double>>>

	int k = Number of successes in the sample
	int n1 = Number of successes in the population
	int n2 = Number of fails in the population
	int t = Size of the sample

	*/
	// The set of genes that are produced by Pool Matching genes intersect Sample genes
	std::map<std::string, int> sample_success = get_sample_success();
	// Calculate the p value with the hypergeometric function from the number of matches , Put it in the genes category map
	for (auto &p : sample_success) {
		// Multithread mutex lock
		double p_value = p_hyper(
			p.second,
			population_success.at(p.first).size(),
			pop_size - population_success.at(p.first).size(),
			sample_size);

		m.lock();
		result[p.first].second.push_back(p_value);
		// Multithread mutex unlock
		m.unlock();
	}
	return true;
}
std::map<std::string, int> FDR::get_sample_success() {
	/*
		Gets called by make_r_sample_result()
		Calculates the random sample intersect population success value.
		Returns the common items.

		TODO Possibly change the rsample generation to check if its in population success
	*/
	std::map<std::string, int> sample_success;
	for (auto &p : population_success) {
		int counter = 0;
		for (std::unordered_set<std::string>::iterator iter = p.second.begin(); iter != p.second.end(); ++iter) {
			if (r_sample.find(*iter) != r_sample.end()) {
				counter++;
			}
		}
		m.lock();
		sample_success[p.first] = counter;
		m.unlock();
	}
	return sample_success;
}
double FDR::p_hyper(unsigned int k, unsigned int n1, unsigned int n2, unsigned int t) {
	/*
	Controlled by make_r_sample_result()
	Calculates the p value from the hypergeometric formula
	
	R phyper() version:
	phyper(q, m, n, k, lower.tail = TRUE, log.p = FALSE)
	
	q =  the number of white balls drawn without replacement from an urn which contains both black and white balls
	m =  the number of white balls in the urn.
	n =  the number of black balls in the urn.
	k =  the number of balls drawn from the urn.
	
	Each ball represents a gene in the population.
	All of the balls together in the urn represents the population.
	The color of the ball represents the inclusion of the gene in the database.
		White balls represents if a gene is in the database.
		Black balls represents if a gene is not in the database.
	The number of balls drawn represents the size of the sample.

	This means:
	q = Number of successes in the sample
	m = Number of successes in the population
	n = Number of fails in the population
	k = Size of the sample

	Which is the same as the GSL version:

	int k = Number of successes in the sample
	int n1 = Number of successes in the population
	int n2 = Number of fails in the population
	int t = Size of the sample
	Returns: p value (double)
	*/
	// Sending in the GSL hyper function
	double i = gsl_cdf_hypergeometric_P(k, n1, n2, t);

	// Calculating p value
	double p_value = 1 - i;
	return p_value;
}