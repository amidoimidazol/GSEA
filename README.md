# GSEA - Gene Set Enrichment Analisys

Using hypergeometric formula to calculate enrichment of certain genes in a differential expression setup.

Hypergeometric GSEA is a simple command line program written in C++ which calculates the hypergeometric probability of different gene sets.

##Dependencies
GSL - Gnu Scientific Library


##Installing GSL:
###Linux:
Download GSL: http://ftp.gnu.org/gnu/gsl/

Unpack the file

    tar -zxvf gsl-2.0.tar.gz 
    
Navigate to the folder you unpacked to, you can give any path you like

    ./configure --prefix=/home/dell/gsl

Navigate to the folder you selected and build

    make

Install

    sudo make install

##Installing GSEA:
###Linux:
You need to compile and build in order to run. I have tested it with g++.
You have to options to compile the program.

####Makefile
Create object file and runnable binary

	make

* You might have to open up the makefile with a text editor and change the line 17 and 22 to reflect the path of your GSL installation.

To clear the folder and remove object files you can use:

	make clean

####Using g++
You might have to change the -L and -I (Library and Include directories )to the path of your gsl installation.

    g++ -o GSEA main.cpp Application_Controller.cpp Thread_Controller.cpp Random_Sample.cpp GSEA.cpp FDR.cpp -pthread -std=c++11 -L/home/dell/gsl/lib -I/home/dell/gsl/include -lgsl -lgslcblas -lm -Wl,--no-as-needed


##Database:
All the database files should be .txt following the GO (gene ontology) format.
For correct use the sample and population files should contain one element per line.
The sample genes must be a subset of the pool genes.
All the databases must follow the same naming convention.

##Use:
If you input a background gene set (pool) and a sample gene set along with a GO gene database the program calculates the probability of enrichment in each go category in the database.

