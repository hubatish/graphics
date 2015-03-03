#include "PSParser.h"

void PSParser::InitialCutting()
{
	//get rid of first part
	const string startString = "%%%BEGIN";
	int pos = fileContents.find(startString);
	if (pos == -1)
	{
		throw 20;
	}
	fileContents.erase(0, pos + startString.size());

	//erase end part
	const string endString = "%%%END";
	pos = fileContents.find(endString);
	if (pos == -1)
	{
		throw 20;
	}
	fileContents.erase(pos, fileContents.size());
}

ZContainer * PSParser::Parse()
{
	ZImage* image = new ZImage();

	try
	{
		ZPoint* curPoint = NULL;
		ZPolygon* curPolygon = NULL;

		for (int i = 0; i < tokens.size(); i++)
		{
			if (tokens[i].compare("line") == 0)
			{
				//we've found a line and expect previous statements to be numbers
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
			else if (tokens[i].compare("moveto") == 0 || tokens[i].compare("lineto") == 0)
			{
				//format of line is:
				//x y moveto/lineto

				int x = stoi(tokens[i - 2]);
				int y = stoi(tokens[i - 1]);
				//set the line to draw from
				curPoint = new ZPoint(x, y);
				if (tokens[i].compare("moveto")==0)
				{
					//create new polygon
					curPolygon = new ZPolygon();
				}
				else
				{
					curPolygon->AddPoint(*curPoint);
				}
			}
			else if (tokens[i].compare("stroke") == 0)
			{
				image->AddPolygon(*curPolygon);
				curPolygon = NULL;
			}
		}
	}
	catch (exception ex)
	{
		cerr << ex.what() << endl;
	}

	return image;
}
