/**************************************************************************************************************
Author: Peter Forgacs <peter.forgacs3@gmail.com>
Date: 2015
Description:	GSEA custom class reads in given files and performs hypergeometric test on the given datasets.
The results are stored in the result global.
***************************************************************************************************************/
#include "GSEA.h"

//----Constructors----//
GSEA::GSEA(char* database_file_, char* population_file_, char* sample_file_)
	: database_file(database_file_), population_file(population_file_), sample_file(sample_file_)
{	
	/*
		This constructor handles the GSEA object.
		Sets the given file paths.
		Calls the make methods in sequence.
	*/
	clock_t tStart = clock();
	make_population();
	printf("make_population: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	
	clock_t tpop_suc = clock();
	make_db_pop();
	printf("population success: %.2fs\n", (double)(clock() - tpop_suc) / CLOCKS_PER_SEC);
	
	clock_t tmake_sample = clock();
	make_sample();
	printf("make_sample: %.2fs\n", (double)(clock() - tmake_sample) / CLOCKS_PER_SEC);
	
	clock_t tsample = clock();
	make_result(sample);
	printf("make_result: %.2fs\n", (double)(clock() - tsample) / CLOCKS_PER_SEC);
	
};

//----Population----//
bool GSEA::make_population() {
	/*
		Controls the reading in from the population file and checks it.
		Creates a vector version that is used by the FDR objects.
	*/
	population = read_file(population_file);
	check_population();
	// Copy set to vector
	std::copy(population.begin(), population.end(), std::back_inserter(v_population));
	return true;
}

//----Database intersect Population----//
void GSEA::make_db_pop() {
	/*
	Gets the database intersect population values.
	Controls the reading in from the database.
	Controls the Subset find between db categories and population -> population_success
	*/
	std::string line;
	std::ifstream myfile(database_file);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			if (!line.empty()) {

				// Produce a database_entries object from the line
				std::string gene;
				std::unordered_set<std::string> genes_in_category;
                                
				std::stringstream ss(line);
				// Getting the category name ( first word on any line ) 
				std::string category_name;
				ss >> category_name;
				// Bug testing echo
				std::cout << category_name << std::endl;

				// Pushing the genes into a set
				while (ss >> gene)
                                        
                                    /*This could potentially fix the line ending bug but probably gets handled somewhere else
                                         while ( gene.find ("\r\n") != string::npos )
                                            {
                                                gene.erase ( gene.find ("\r\n"), 2 );
                                            }
                                    */
                                    
					genes_in_category.insert(gene);
                                
				// Remove category name from the set
				genes_in_category.erase(category_name);

				// Find common genes between category and pool
				find_common_genes(category_name, genes_in_category);
			}
		}
		myfile.close();
	}
	//else throw GSEA_EXCEPTION("The file cant be opened: ", __FILE__, __LINE__, __FUNCTION__, database_file);
}
void GSEA::find_common_genes(const std::string& category_name, const std::unordered_set<std::string>& genes_in_category) {
	/*
		Called by make_db_pop
		Fills in the population_success global variable.
		Finds the common elements between the database category and the population.
		Stores the category name and the common genes in the population_success global variable.
	*/
	std::unordered_set<std::string> common_genes_in_category;

	for (std::unordered_set<std::string>::const_iterator it = genes_in_category.begin(); it != genes_in_category.end(); it++) {
		if (population.find(*it) != population.end()) {
			common_genes_in_category.insert(*it);
		}
	}
	population_success[category_name] = common_genes_in_category;
}

//----Sample----//
bool GSEA::make_sample() {
	/*
		Reads in the sample file and checks if its correct.
	*/
	// Read in sample from file
	sample = read_file(sample_file);
	sample_size = sample.size();
	check_sample();
	return true;
}

//----Hypergeometric methods----//
bool GSEA::make_result(std::unordered_set<std::string>& sample){
	/*
	Controls the checking of the intersect of the population success and sample.
	Fills in the result global map with the appropriate values.

	sample_success			|	std::map<std::string, int>
	population_success		|	std::map<std::string, std::unordered_set<std::string>>
	result					|	std::map<std::string, std::pair<double, std::vector<double>>>
	*/

	// The set of genes that are produced by Pool Matching genes intersect Sample genes
	std::map<std::string, int> sample_success = get_sample_success(sample);
	// Calculate the p value with the hypergeometric function from the number of matches , Put it in the result global map.
	for (auto const &p : sample_success){
		result[p.first].first = p_hyper(p.second,
			population_success.at(p.first).size(),
			population.size() - population_success.at(p.first).size(),
			sample_size);
	}
	return true;
}
std::map<std::string, int> GSEA::get_sample_success(std::unordered_set<std::string>& sample){
	/*
		Controlled by make_result()
		Gets the population success intersect sample value.
		Returns the number of common elements.
	*/
	std::map<std::string, int> sample_success;
	for (auto &p : population_success) {
		int counter = 0;
		for (std::unordered_set<std::string>::iterator iter = p.second.begin(); iter != p.second.end(); ++iter) {
			if (sample.find(*iter) != sample.end()){
				counter++;
			}
		}
		sample_success[p.first] = counter;
	}
	return sample_success;
}
double GSEA::p_hyper(unsigned int k, unsigned int n1, unsigned int n2, unsigned int t){
	/*
	Controlled by make_result()

	Calculates the p value from the hypergeometric formula
	Input:
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

//----Error checking methods----//
void GSEA::check_population() {
	/*
	Checking the given population file for problems.
	*/
	//if (population.size() <= 0)
		//throw GSEA_EXCEPTION("The population file contains no genes.", __FILE__, __LINE__, __FUNCTION__);
}
void GSEA::check_sample() {
	/*
	Checking the given sample file for problems.
	*/

	// Check if sample is not empty
	if (sample_size <= 0)
		throw GSEA_EXCEPTION("The sample file contains no genes.", __FILE__, __LINE__, __FUNCTION__);

	// Check if the sample is a subset of the population 
	for (std::unordered_set<std::string>::const_iterator it = sample.begin(); it != sample.end(); it++) {
		if (population.find(*it) == population.end()) {
			//throw GSEA_EXCEPTION("The sample is not a subset of the population.", __FILE__, __LINE__, __FUNCTION__);
		}
	}
}

//----Generic methods----//
std::unordered_set<std::string> GSEA::read_file(const char* file_path) {
	/*
	Reads into a given file, it supposed to read one element from each line
	TODO if there are multiple values in a line it handles it incorrectly
	TODO place it as a general function outside of the class
	*/
	std::unordered_set<std::string> container;
	std::string line;
	std::ifstream file(file_path);

	if (file.is_open()) {
		while (getline(file, line)) {
			if (!line.empty()) {

				container.insert(line);
			}
		}
		file.close();
		return container;
	}
	//else throw GSEA_EXCEPTION("The file cannot be opened: ", __FILE__, __LINE__, __FUNCTION__, file_path);

}

//----Display methods----//
void GSEA::display_set(std::unordered_set<std::string> & set){
	/*
		Prints out elements of a given set
	*/
	for (std::unordered_set<std::string>::iterator it = set.begin(); it != set.end(); it++){
		std::cout << *it << std::endl;
	}
}
void GSEA::display_common(std::map<std::string, std::unordered_set<std::string>> & common){
	/*
		Display results
	*/
	for (auto &p : population_success) {
		std::cout << p.first << ":";
		for (std::unordered_set<std::string>::iterator iter = p.second.begin(); iter != p.second.end(); ++iter) {
			std::cout << *iter << " ";
		}
		std::cout << std::endl;
	}
}

