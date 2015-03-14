#include "XPMOutput.h"


XPMOutput::XPMOutput(ZRect * rect)
{
	if (rect == NULL)
	{
		rect = new ZRect();
	}
	bounds = *rect;

	//need this for offsets
	negLowerBound = bounds.lowerBound;
	negLowerBound.Scale(-1);

	//size initial grid based on bounds
	int width = bounds.GetWidth()+1;
	int height = bounds.GetHeight()+1;
	
	//initialize the grid to white
	grid.resize(width);
	for (int i = 0; i < width; i++)
	{
		grid[i].resize(height);
		for (int j = 0; j < height; j++)
		{
			grid[i][j] = PointOnGrid();
			grid[i][j].c = Color::WHITE;
			grid[i][j].p = *(new ZPoint());
		}
	}
}


XPMOutput::~XPMOutput()
{
}

void XPMOutput::Output(ostream* out)
{
	const string startingLines = "/* XPM */ \n static char *sco100[] = { \n \n /* width height num_colors chars_per_pixel */ \n";
	*out << startingLines << endl;
	int width = bounds.GetWidth()+1;
	int height = bounds.GetHeight()+1;
	*out << "\"" << width << " " << height << " " << num_colors << " " << chars_per_pixel << "\"," << endl;
	*out << "/* colors */" << endl;
	*out << "\"" << static_cast<char>(Color::BLACK) << " c #000000\"," << endl;
	*out << "\"" << static_cast<char>(Color::WHITE) << " c #ffffff\"," << endl;
	*out << "/* pixels */" << endl;
	for (int y = height-1; y >=0; y--)
	{
		*out << "\"";
		for (int x = 0; x < width; x++)
		{
			*out << static_cast<char>(grid[x][y].c);
		}
		*out << "\"";
		if (y != height)
		{
			*out << ",";
		}
		*out << endl;
	}
	*out << "};" << endl;
}

void XPMOutput::DrawPoint(ZPoint point, Color color)
{ 
	//offset point so lowerBound is always at 0,0
	OffsetPoint(point);
	//Update to allow for z values
	if (point.z>grid[point.x][point.y].p.z)
	{
		grid[point.x][point.y].c = color;
		grid[point.x][point.y].p = point;
	}
}


///Draw a line onto pixels
///While loop algorithm pseudo code from here: http://ocw.unican.es/ensenanzas-tecnicas/visualizacion-e-interaccion-grafica/material-de-clase-2/03-LineAlgorithms.pdf
void XPMOutput::DrawLine(ZLine line, Color color)
{
	//DDA line drawing algorithm
	float m = line.GetSlope();
	
	float z = line.startPoint.z;

	if (-1 < m && m < 1)
	{
		//use x
		int x, xRight, y;
		float yTrue;

		//choose which point is lower
		ZPoint minPoint, maxPoint;
		if (line.startPoint.x < line.endPoint.x)
		{
			minPoint = line.startPoint;
			maxPoint = line.endPoint;
		}
		else
		{
			minPoint = line.endPoint;
			maxPoint = line.startPoint;
		}

		x = minPoint.x;
		y = yTrue = minPoint.y;
		xRight = maxPoint.x;

		while (x < xRight)
		{
			DrawPoint(ZPoint(x, y, z), color);
			yTrue = yTrue + m;
			y = round(yTrue);
			x++;
		}
	}
	else
	{
		//steep line, use y
		int y, yTop, x;
		float xTrue;

		//this check is not strictly necessary but it looks clean
		if (ZLine::IsSlopeVertical(m))
		{
			m = 0;
		}
		else
		{
			m = 1.0 / m;
		}

		//choose which point is lower
		ZPoint minPoint, maxPoint;
		if (line.startPoint.y < line.endPoint.y)
		{
			minPoint = line.startPoint;
			maxPoint = line.endPoint;
		}
		else
		{
			minPoint = line.endPoint;
			maxPoint = line.startPoint;
		}

		y = minPoint.y;
		x = xTrue = minPoint.x;
		yTop = maxPoint.y;

		while (y < yTop)
		{
			DrawPoint(ZPoint(x, y, z), color);
			xTrue = xTrue + m;
			x = round(xTrue);
			y++;
		}
	}
}


void XPMOutput::DrawPolygon(ZPolygon & polygon, Color color)
{
	if (polygon.points.size() < 2)
	{
		return;
	}
	ZPoint * prev = &polygon.points[polygon.points.size()-1];
	for (int i = 0; i < polygon.points.size(); i++)
	{
		DrawLine(ZLine(polygon.points[i], *prev), color);
		prev = &polygon.points[i];
	}
}

void XPMOutput::FillPolygon(ZPolygon & polygon, Color color)
{
	if (polygon.points.size() <= 0)
	{
		//don't process non-existant polygons
		return;
	}

	//Hacky addition of z - 3-13
	float z = polygon.points[0].z;

	//Create intersectingLines map - which edges intersect a scan line
	map<int,vector<ZLine>> intersectingLines;
	ZPoint * prev = &polygon.points[polygon.points.size() - 1];
	for (int i = 0; i < polygon.points.size(); i++)
	{
		ZLine curEdge = ZLine(*prev, polygon.points[i]);
		int yMin, yMax;
		yMin = min(curEdge.startPoint.y, curEdge.endPoint.y);
		yMax = max(curEdge.startPoint.y, curEdge.endPoint.y);
		//add this edge to all applicable scan lines
		for (int curY = yMin; curY < yMax;curY++)
		{
			intersectingLines[curY].push_back(curEdge);
		}
		prev = &polygon.points[i];
	}

	//actually find intersections & draw lines
	for (auto iter : intersectingLines)
	{
		float curY = (float)iter.first;
		vector<ZLine> lines = iter.second;

		//find which xs intersect the line (comment added 3-13)
		vector<float> intersectionXs;
		for (auto line : lines)
		{
			int curX;
			float reverseM = ((float)line.endPoint.x - (float)line.startPoint.x) / ((float)line.endPoint.y - (float)line.startPoint.y);
			curX = floor((float)line.startPoint.x + reverseM* (float)(curY - (float)line.startPoint.y));
			intersectionXs.push_back(curX);
		}

		//sort the xs
		sort(intersectionXs.begin(), intersectionXs.end());
		int * prevX = NULL;
		for (auto x : intersectionXs)
		{
			if (prevX == NULL)
			{
				prevX = new int(x);
			}
			else
			{
				//handle duplicates???????
				ZLine toDraw = ZLine(ZPoint(*prevX, curY,z), ZPoint(x, curY,z));
				DrawLine(toDraw, color);
				prevX = NULL;
			}
		}
	}
}

void XPMOutput::DrawImage(ZImage & image, Color color)
{
	for (int i = 0; i < image.lines.size(); i++)
	{
		DrawLine(image.lines[i],color);
	}
	for (int i = 0; i < image.polygons.size(); i++)
	{
		DrawPolygon(image.polygons[i], color);
	}
}

