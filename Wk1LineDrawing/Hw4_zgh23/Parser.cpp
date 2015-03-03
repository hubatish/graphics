#include "Parser.h"

Parser::Parser()
{
}

void Parser::Initialize(string f)
{
	try
	{
		fstream file;
		file.open(f);
		fileContents = *(new string((istreambuf_iterator<char>(file)),
			istreambuf_iterator<char>()));

		InitialCutting();

		string splitters = "\n\ ";
		Tokenize(splitters);

	}
	catch (exception ex)
	{
	}
}

Parser::~Parser()
{
}

void Parser::Tokenize(string c)
{
	istringstream iss(fileContents);
	copy(istream_iterator<string>(iss),
		istream_iterator<string>(),
		back_inserter(tokens));

	for (int i = 0; i < tokens.size(); i++)
	{
		MakeStringLower(&tokens[i]);
	}
}

void Parser::MakeStringLower(string * str)
{
	for (int i = 0; i < str->size(); i++)
	{
		string yo = (*str);
		(*str)[i] = tolower((*str)[i]);
	}
}

