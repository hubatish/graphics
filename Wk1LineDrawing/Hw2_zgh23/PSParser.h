#ifndef XPM_OUTPUT_H
#define XPM_OUTPUT_H

#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <exception>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "ZPoint.h"
using namespace std;

class PSParser
{
	string fileContents;

public:
	PSParser(string f);
	~PSParser();

	void Tokenize(string c);
	ZImage* ParseLines();
	void MakeStringLower(string *str);

	vector<string> tokens;
};

#endif