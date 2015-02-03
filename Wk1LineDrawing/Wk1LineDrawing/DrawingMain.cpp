// Wk1LineDrawing.cpp : Defines the entry point for the console application.
//

#include "PSParser.h"
#include "ZPoint.h"
#include "XPMOutput.h"
#include <iostream>

void TestLineDraws(XPMOutput& xpm);
void TestTranslations(ZImage& image);

int main(int argc, char* argv[])
{
	//argument things
	string fileName = "hw2/hw2_c.ps";
	ZPoint * lowerBound = NULL;
	ZPoint * upperBound = NULL;
	float scale = 1.0;
	float degreeAngle = 0;
	float translateX = 0;
	float translateY = 0;

	//Parse command line arguments
	for (int i = 0; i < argc - 1; i ++)
	{
		string curArg(argv[i]);
		if (curArg.compare("-f")==0)
		{
			fileName = argv[i + 1];
		}
		else if (curArg.compare("-a")==0)
		{
			//lower bound x dimension world window
			if (lowerBound == NULL)
			{
				lowerBound = new ZPoint(0, 0);
			}
			lowerBound->x = stoi(argv[i + 1]);
		}
		else if (curArg.compare("-b")==0)
		{
			if (lowerBound == NULL)
			{
				lowerBound = new ZPoint(0, 0);
			}
			lowerBound->y = stoi(argv[i + 1]);
		}
		else if (curArg.compare("-c") == 0)
		{
			if (upperBound == NULL)
			{
				upperBound = new ZPoint(0, 0);
			}
			upperBound->x = stoi(argv[i + 1]);
		}
		else if (curArg.compare("-d") == 0)
		{
			if (upperBound == NULL)
			{
				upperBound = new ZPoint(0, 0);
			}
			upperBound->y = stoi(argv[i + 1]);
		}
		else if (curArg.compare("-s") == 0)
		{
			scale = stof(argv[i + 1]);
		}
		else if (curArg.compare("-r") == 0)
		{
			degreeAngle = stof(argv[i + 1]);
		}
		else if (curArg.compare("-m") == 0)
		{
			translateX = stof(argv[i + 1]);
		}
		else if (curArg.compare("-n") == 0)
		{
			translateY = stof(argv[i + 1]);
		}
	}

	PSParser parser = PSParser(fileName);

	ZImage * image = parser.ParseLines();

	if (scale != 1.0)
	{
		image->Scale(scale);
	}
	if (degreeAngle != 0)
	{
		image->Rotate(degreeAngle);
	}
	if (translateX != 0)
	{
		image->Translate(ZPoint(translateX,0));
	}
	if (translateY != 0)
	{
		image->Translate(ZPoint(0, translateY));
	}

	XPMOutput xpm(lowerBound,upperBound);

	TestLineDraws(xpm);
	xpm.DrawImage(*image, Color::BLACK);

	ofstream fout;
	fout.open("out.xpm");
	xpm.Output(&cout);

	return 0;
}

void TestLineDraws(XPMOutput& xpm)
{
	//xpm.DrawPoint(ZPoint(0, 0), XPMOutput::Color::BLACK);
//	xpm.DrawLine(ZLine(ZPoint(0, 0), ZPoint(10, 10)), XPMOutput::Color::BLACK);
	//xpm.DrawLine(ZLine(ZPoint(0, 0), ZPoint(30, 10)), XPMOutput::Color::BLACK);
	//xpm.DrawLine(ZLine(ZPoint(-10, -10), ZPoint(10, 10)), XPMOutput::Color::BLACK);
	//xpm.DrawLine(ZLine(ZPoint(25, 0), ZPoint(25, 10)), XPMOutput::Color::BLACK);
}
