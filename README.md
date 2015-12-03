# GSEA - Gene Set Enrichment Analisys

Using hypergeometric formula to calculate enrichment of certain genes in a differential expression setup.

Hypergeometric GSEA is a simple command line program written in C++ which calculates the hypergeometric probability of different gene sets.

##Dependencies
GSL - Gnu Scientific Library

TCLAP - Command line parser

##Installing GSL:
###Linux:
Download GSL: http://ftp.gnu.org/gnu/gsl/

Unpack the file

    gsl-2.0$ tar -zxvf gsl-2.0.tar.gz 
    
Navigate to the folder you unpacked to, you can give any path you like

    gsl-2.0$ ./configure --prefix=/home/dell/gsl

Navigate to the folder you selected and build

    make

Install

    sudo make install

##Installing GSEA:
###Linux:
You need to compile and build in order to run. I have tested it with g++.
You might have to change the -L and -I (Library and Include directories )to the path of your gsl installation.

    g++ -o GSEA hypergeometric.cpp GSEA.cpp FDR.cpp -pthread -std=c++11 -L/home/dell/gsl/lib -I/home/dell/gsl/include -lgsl -lgslcblas -lm -I./header -Wl,--no-as-needed


##Database:
All the database files should be .txt following the GO (gene ontology) format.
For correct use the sample and population files should contain one element per line.
The sample genes must be a subset of the pool genes.
All the databases must follow the same naming convention.

##Use:
If you input a background gene set (pool) and a sample gene set along with a GO gene database the program calculates the probability of enrichment in each go category in the database. 

##Commands:
Running the test with the predefined values:

    hypergeometric
    
List of commands:

    hypergeometric -h

Manual input of database and genes:

    hypergeometric -m

Define costume database sample and pool:

    hypergeometric -f -d database.txt -s sample.txt -p pool.txt

