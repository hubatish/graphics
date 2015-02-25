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

class Parser
{
public:
	Parser();
	~Parser();

	void Initialize(string f);
	void Tokenize(string c);
	virtual void InitialCutting() = 0;
	virtual ZImage* ParseLines() = 0;
	void MakeStringLower(string *str);

	string fileContents;
	vector<string> tokens;
};
