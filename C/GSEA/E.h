/********************************************************************************************************************************
Author: Peter Forgacs <peter.forgacs3@gmail.com>
Date: 2015
Description: Custom exception that returns the file name / line / function and addition iformation if given about the exception.
*********************************************************************************************************************************/
#pragma once
#include <exception>
#include <stdexcept>
using namespace std;

class GSEA_EXCEPTION : public std::runtime_error {

	const char* file;
	int line;
	const char* func;
	const char* info;

public:
	GSEA_EXCEPTION(const char* msg, const char* file_, int line_, const char* func_, const char* info_ = "")
		: std::runtime_error(msg)
		, file(file_)
		, line(line_)
		, func(func_)
		, info(info_)
	{
	}

	const char* get_file() const { return file; }
	int get_line() const { return line; }
	const char* get_func() const { return func; }
	const char* get_info() const { return info; }
};


