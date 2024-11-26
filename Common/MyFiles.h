#pragma once

//Wrapper for graphic loading, do not use for binary or text files.
#include <fstream>
#include <iostream> // we're going to be loading files so need these
#include <string>
using namespace std;

class FileLoader
{
public:
	FileLoader();
	~FileLoader();

	int height;
	int width;
	int comp;

	char* StbiLoad(char const* filename, int*, int*); // specifically for graphics

	// !used
	const char* LoadText(char const* filename);
};
