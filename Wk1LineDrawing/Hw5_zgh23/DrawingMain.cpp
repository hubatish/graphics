// Wk1LineDrawing.cpp : Defines the entry point for the console application.
//

#include "PSParser.h"
#include "ZPoint.h"
#include "XPMOutput.h"
#include "SMFParser.h"
#include "ViewVolume.h"
#include <iostream>

void TestLineDraws(XPMOutput& xpm);
void TestTranslations(ZImage& image);
void ProcessImage(const string & fileName, ZRect * viewPortRect, XPMOutput & xpm, Color color);

int main(int argc, char* argv[])
{
	//argument things
	string fileName = "hw4/bound-sprellpsd.smf";
	string greenFileName = "";
	string blueFileName = "";
	ZRect * desktopRect = new ZRect(new ZPoint(0, 0), new ZPoint(500, 500));
	ZRect * viewPortRect = new ZRect(new ZPoint(0,0),new ZPoint(500,500));
	float scale = 1.0;
	float degreeAngle = 0;
	float translateX = 0;
	float translateY = 0;
	bool useParallel = false;

	argsViewVolume = ViewVolume();

	//Parse command line arguments
	for (int i = 0; i < argc - 1; i ++)
	{
		string curArg(argv[i]);
		if (curArg.compare("-f")==0)
		{
			fileName = argv[i + 1];
		}
		if (curArg.compare("-g") == 0)
		{
			greenFileName = argv[i + 1];
		}
		if (curArg.compare("-i") == 0)
		{
			blueFileName = argv[i + 1];
		}
		//Check window bounds
		else if (curArg.compare("-a")==0)
		{
			//lower bound x dimension world window
			desktopRect->lowerBound.x = stoi(argv[i + 1]);
		}
		else if (curArg.compare("-b")==0)
		{
			desktopRect->lowerBound.y = stoi(argv[i + 1]);
		}
		else if (curArg.compare("-c") == 0)
		{
			desktopRect->upperBound.x = stoi(argv[i + 1]);
		}
		else if (curArg.compare("-d") == 0)
		{
			desktopRect->upperBound.y = stoi(argv[i + 1]);
		}
		//Check viewport bounds
		else if (curArg.compare("-j") == 0)
		{
			//lower bound x dimension world window
			if (viewPortRect == NULL)
			{
				viewPortRect = new ZRect();
			}
			viewPortRect->lowerBound.x = stoi(argv[i + 1]);
		}
		else if (curArg.compare("-k") == 0)
		{ 
			if (viewPortRect == NULL)
			{
				viewPortRect = new ZRect();
			}
			viewPortRect->lowerBound.y = stoi(argv[i + 1]);
		}
		else if (curArg.compare("-o") == 0)
		{
			if (viewPortRect == NULL)
			{
				viewPortRect = new ZRect();
			}
			viewPortRect->upperBound.x = stoi(argv[i + 1]);
		}
		else if (curArg.compare("-p") == 0)
		{
			if (viewPortRect == NULL)
			{
				viewPortRect = new ZRect();
			}
			viewPortRect->upperBound.y = stoi(argv[i + 1]);
		}
		else if (curArg.compare("-P") == 0 || ((string)argv[i + 1]).compare("-P") == 0)
		{
			useParallel = true;
			argsViewVolume.parallel = true;
		}
		//Transformations
		/*else if (curArg.compare("-s") == 0)
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
		}*/
		argsViewVolume.ParseArg(argv[i], argv[i + 1]);
	}

	XPMOutput xpm(desktopRect);

	if (fileName.size() > 0)
	{
		ProcessImage(fileName, viewPortRect, xpm, Color::RED);
	}

	if (greenFileName.size() > 0)
	{
		ProcessImage(greenFileName, viewPortRect, xpm, Color::GREEN);
	}
	if (blueFileName.size() > 0)
	{
		//green weorks, blue doesn't
		ProcessImage(blueFileName, viewPortRect, xpm, Color::BLUE);
	}

	ofstream fout;
	fout.open("out.xpm");
	xpm.Output(&cout);

	return 0;
}

void ProcessImage(const string & fileName, ZRect * viewPortRect, XPMOutput & xpm, Color color)
{
	bool isPSFile = (fileName.find(".ps") != string::npos);

	Parser * parser;
	if (isPSFile)
	{
		parser = new PSParser();
	}
	else
	{
		parser = new SMFParser();
	}

	parser->Initialize(fileName);

	ZContainer * image = parser->Parse();

	argsViewVolume.Project(*image);

	BoundedImage imageInWorld((ZImage*)image, argsViewVolume.GetCanonicalRect());
	BoundedImage * imageInWindow;
	if (viewPortRect == NULL)
	{
		imageInWindow = &imageInWorld;
	}
	else
	{
		imageInWindow = imageInWorld.FitToViewort(*viewPortRect);
	}

	xpm.DrawImage(imageInWindow->image, color);

}

void TestLineDraws(XPMOutput& xpm)
{
	//xpm.DrawPoint(ZPoint(0, 0), XPMOutput::Color::BLACK);
//	xpm.DrawLine(ZLine(ZPoint(0, 0), ZPoint(10, 10)), XPMOutput::Color::BLACK);
	//xpm.DrawLine(ZLine(ZPoint(0, 0), ZPoint(30, 10)), XPMOutput::Color::BLACK);
	//xpm.DrawLine(ZLine(ZPoint(-10, -10), ZPoint(10, 10)), XPMOutput::Color::BLACK);
	//xpm.DrawLine(ZLine(ZPoint(25, 0), ZPoint(25, 10)), XPMOutput::Color::BLACK);

	//testing 
//	ZPolygon poly = imageInWindow->image.polygons[0];
//	ZPoint toMove(4000, 3000);
//	poly.ApplyFunction(ZPoint::TranslateP, toMove);

}

void TestPointTransformation()
{
	ZPoint p1(5, 5, 10);
	Matrix4f m;
	m <<
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 1, 0;
	p1.Transform(m);
	float skekek = 3;
}