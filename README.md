# GSEA - Gene Set Enrichment Analisys

Using hypergeometric formula to calculate enrichment of certain genes in a differential expression setup.

Hypergeometric GSEA is a simple command line program written in C++ which calculates the hypergeometric probability of different gene sets.

## Dependencies
GSL - Gnu Scientific Library


## Installing GSL:
### Linux:
Download GSL: http://ftp.gnu.org/gnu/gsl/

Create a directory where the library will be installed to.
Unpack the file into a different directory. 
```sh
    tar -zxvf gsl-2.0.tar.gz 
```    
Navigate to the folder you unpacked to, the prefix must be the path to the final installation directory.
```sh
    ./configure --prefix=/home/dell/gsl
```
Run make
```sh
    make
```
Install
```sh
    make install
```
The include files will be in the directory set in the prefix.
The install can fail if the install directory contains spaces in their path!

## Installing GSEA:
### Linux:
You need to compile and build in order to run. I have tested it with g++.
You have two options to compile the program.

#### Makefile
Create object file and runnable binary
```sh
	make
```

You might have to open up the makefile with a text editor and change the line 17 and 22 to reflect the path of your GSL installation.

To clear the folder and remove object files you can use:
```sh
	make clean
```
#### Using g++
You might have to change the -L and -I (Library and Include directories )to the path of your gsl installation.
```sh
    g++ -o GSEA main.cpp Application_Controller.cpp Thread_Controller.cpp Random_Sample.cpp GSEA.cpp FDR.cpp -pthread -std=c++11 -L/home/dell/gsl/lib -I/home/dell/gsl/include -lgsl -lgslcblas -lm -Wl,--no-as-needed
```
## Using GSEA:
If you input a background gene set (pool) and a sample gene set along with a GO gene database the program calculates the probability of enrichment in each go category in the database.
As of right now the databases can only be changed from the source.

After you compiled the program simply use:
```sh
    ./GSEA ./Database/medium_db.txt ./Database/medium_pool.txt ./Database/medium_sample.txt
```
You can view the results of the FDR test:
```sh
    cat ./Log/result.txt  
```
## Database formats:
All the database files should be .txt following the GO (gene ontology) format.
For correct use the sample and population files should contain one element per line.
The sample genes must be a subset of the pool genes.
All the databases must follow the same naming convention.
All the datanase files must use the same line endings either UNIX or Windows.
