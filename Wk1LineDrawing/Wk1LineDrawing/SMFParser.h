#pragma once
#include "Parser.h"
#include "ZMesh.h"
using namespace std;

class SMFParser : public Parser
{
public:
	SMFParser() :Parser(){}

	virtual void InitialCutting(){}
	virtual ZContainer* Parse();
};
