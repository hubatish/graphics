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
	string fileName = "hw1.ps";
	ZPoint * lowerBound = NULL;
	ZPoint * upperBound = NULL;
	float* scale = NULL;
	float* degreeAngle = NULL;
	float* translateX = NULL;
	float* translateY = NULL;

	//Parse command line arguments
	for (int i = 0; i < argc - 1; i += 2)
	{
		if ("-f" == argv[i])
		{
			fileName = argv[i + 1];
		}
		else if ("-a" == argv[i])
		{
			//lower bound x dimension world window
			if (lowerBound == NULL)
			{
				lowerBound = new ZPoint(0, 0);
			}
			lowerBound->x = stoi(argv[i + 1]);
		}
		else if ("-b" == argv[i])
		{
			if (lowerBound == NULL)
			{
				lowerBound = new ZPoint(0, 0);
			}
			lowerBound->y = stoi(argv[i + 1]);
		}
		else if ("-c" == argv[i])
		{
			if (upperBound == NULL)
			{
				upperBound = new ZPoint(0, 0);
			}
			upperBound->x = stoi(argv[i + 1]);
		}
		else if ("-d" == argv[i])
		{
			if (upperBound == NULL)
			{
				upperBound = new ZPoint(0, 0);
			}
			upperBound->y = stoi(argv[i + 1]);
		}
		else if ("-s" == argv[i])
		{
			*scale = stoi(argv[i + 1]);
		}
		else if ("-r" == argv[i])
		{
			*degreeAngle = stoi(argv[i + 1]);
		}
		else if ("-m" == argv[i])
		{
			*translateX = stoi(argv[i + 1]);
		}
		else if ("-n" == argv[i])
		{
			*translateY = stoi(argv[i + 1]);
		}
	}

	PSParser parser = PSParser(fileName);
	string splitters = "\n\ ";
	parser.Tokenize(splitters);

	ZImage * image = parser.ParseLines();

	if (scale != NULL)
	{
		image->Scale(*scale);
	}
	if (degreeAngle != NULL)
	{
		image->Rotate(*degreeAngle);
	}
	if (translateX != NULL)
	{
		image->Translate(ZPoint(*translateX,0));
	}
	if (translateY != NULL)
	{
		image->Translate(ZPoint(0, *translateY));
	}

	XPMOutput xpm(lowerBound,upperBound);

	TestLineDraws(xpm);
	xpm.DrawImage(*image, XPMOutput::Color::BLACK);

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
