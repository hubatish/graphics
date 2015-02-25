#include "SMFParser.h"

ZContainer * SMFParser::Parse()
{
	ZImage* image = new ZImage();

	try
	{
		vector<ZPoint> points;

		for (int i = 0; i < tokens.size(); i++)
		{
			if (tokens[i].compare("v") == 0)
			{
				//format of line is:
				//v x y z

				float x = stof(tokens[i + 1]);
				float y = stof(tokens[i +2]);
				float z = stof(tokens[i + 3]);
				//set the line to draw from
				ZPoint * point = new ZPoint(x, y, z);
				points.push_back(*point);
			}
			else if (tokens[i].compare("f") == 0)
			{
				//format of line is
				//f p0 p1 p2

				ZPolygon * polygon = new ZPolygon();

				int p0 = stoi(tokens[i + 1]) - 1;
				int p1 = stoi(tokens[i + 2]) - 1;
				int p2 = stoi(tokens[i + 3]) - 1;
				polygon->AddPoint(points[p0]);
				polygon->AddPoint(points[p1]);
				polygon->AddPoint(points[p2]);
				image->AddPolygon(*polygon);
			}
		}
	}
	catch (exception ex)
	{
		cerr << ex.what() << endl;
	}

	return image;
}
