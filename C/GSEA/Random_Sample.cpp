/******************************************************************************************************
Author: Peter Forgacs <peter.forgacs3@gmail.com>
Date: 2015
Description:The R_sample class is a container class.
It gets used by the Thread Controller and FDR classes to create and store
the random samples that are needed for the FDR test.
It uses Fisher Yates shuffle to create random samples from the original population.
The size of the r_sample container depends on the number of threads and repetition number.
*******************************************************************************************************/
#include "Random_Sample.h"

R_sample::R_sample(int sample_size_, int obj_rep_num_, vector<string> & pop_) : sample_size(sample_size_), obj_rep_num(obj_rep_num_), pop(pop_)
{	/*
		Constructor that calls the creation of the random samples.
	*/
	pop_size = pop.size();
	make_r_samples();
};

void R_sample::make_r_samples()
{	
	/*
		Generates random samples.
		The product is vector of unordered sets with random samples
	*/

	// Loop trough the vector
	for (int i = 0; i < obj_rep_num; ++i)
	{	
		// Save the original population (Shuffle changes it)
		backup_pop = pop;
		std::unordered_set<string> container_set;

		// Generate random sample
		random_unique(pop.begin(), pop.end(), sample_size);

		// Insert into a set
		for (int j = 0; j < sample_size; ++j) {
			container_set.insert(pop[j]);
		}

		// Place new random sample in container
		r_samples.push_back(container_set);
		
		// Restore original pop
		pop = backup_pop;
	}
}

template<class bidiiter>
bidiiter R_sample::random_unique(bidiiter begin, bidiiter end, size_t num_random) {
	/*
	Gets called by make_r_sample()
	Randomizes a sample

	Uses Fisher Yates shuffle
	http://www.wikiwand.com/en/Fisher%E2%80%93Yates_shuffle
	http://stackoverflow.com/questions/9345087/choose-m-elements-randomly-from-a-vector-containing-n-elements

	*/
	size_t left = std::distance(begin, end);
	while (num_random--) {
		bidiiter r = begin;
		std::advance(r, rand() % left);
		std::swap(*begin, *r);
		++begin;
		--left;
	}
	return begin;
}