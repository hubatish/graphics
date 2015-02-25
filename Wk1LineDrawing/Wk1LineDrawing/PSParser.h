#ifndef PS_PARSER_H
#define PS_PARSER_H

#pragma once
#include "Parser.h"
using namespace std;

class PSParser : public Parser
{
public:
	PSParser():Parser(){}

	virtual void InitialCutting();
	virtual ZImage* ParseLines();
};

#endif