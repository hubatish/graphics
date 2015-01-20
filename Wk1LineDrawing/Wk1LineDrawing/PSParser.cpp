#include "stdafx.h"
#include "PSParser.h"

PSParser::PSParser(string f)
{
	try
	{
		fstream file;
		file.open(f);
		fileContents = *(new string((istreambuf_iterator<char>(file)),
			istreambuf_iterator<char>()));

		//get rid of first part
		const string startString = "%%%BEGIN";
		int pos = fileContents.find_first_of(startString);
		if (pos == -1)
		{
			throw 20;
		}
		fileContents.erase(pos, startString.size() );

		//erase end part
		const string endString = "%%%END";
		pos = fileContents.find(endString);
		if (pos == -1)
		{
			throw 20;
		}
		fileContents.erase(pos, fileContents.size());
		cout << fileContents << endl;
	}
	catch (exception ex)
	{
	}
}


PSParser::~PSParser()
{
}

void PSParser::Tokenize(string c)
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

void PSParser::MakeStringLower(string * str)
{
	for (int i = 0; i < str->size(); i++)
	{
		string yo = (*str);
		(*str)[i] = tolower((*str)[i]);
	}
}

ZImage * PSParser::ParseLines()
{
	ZImage* image = new ZImage();
	for (int i = 0; i < tokens.size(); i++)
	{
		if (tokens[i].compare("line") == 0)
		{
			//we've found a line and expect previous statements to be numbers
			try
			{
				//format of line is:
				//x1 y1 x2 y2 LINE
				//so use previous tokens
				int x1, y1, x2, y2;
				x1 = stoi(tokens[i - 4]);
				y1 = stoi(tokens[i - 3]);
				x2 = stoi(tokens[i - 2]);
				y2 = stoi(tokens[i - 1]);
				ZPoint * lower = new ZPoint(x1, y1);
				ZPoint * upper = new ZPoint(x2, y2);
				ZLine * newLine = new ZLine(*lower, *upper);
				image->AddLine(*newLine);
			}
			catch (exception ex)
			{
				cerr << ex.what() << endl;
			}
		}
	}
	return image;
}