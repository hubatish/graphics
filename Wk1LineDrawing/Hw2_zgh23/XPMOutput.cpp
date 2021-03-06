#include "XPMOutput.h"


XPMOutput::XPMOutput(ZPoint* lowerBound, ZPoint * upperBound)
{
	//handle optional arguments
	if (lowerBound == NULL)
	{
		this->lowerBound = ZPoint(0, 0);
	}
	else
	{
		this->lowerBound = *lowerBound;
	}
	if (upperBound == NULL)
	{
		this->upperBound = ZPoint(499, 499);
	}
	else
	{
		this->upperBound = *upperBound;
	}

	//need this for offsets
	negLowerBound = this->lowerBound;
	negLowerBound.Scale(-1);

	//size initial grid based on bounds
	int width = GetWidth();
	int height = GetHeight();
	
	//initialize the grid to white
	grid.resize(width);
	for (int i = 0; i < width; i++)
	{
		grid[i].resize(height);
		for (int j = 0; j < height; j++)
		{
			grid[i][j] = Color::WHITE;
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
	int width = GetWidth();
	int height = GetHeight();
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
			*out << static_cast<char>(grid[x][y]);
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
	grid[point.x][point.y] = color; 
}


///Draw a line onto pixels
///While loop algorithm pseudo code from here: http://ocw.unican.es/ensenanzas-tecnicas/visualizacion-e-interaccion-grafica/material-de-clase-2/03-LineAlgorithms.pdf
void XPMOutput::DrawLine(ZLine line, Color color)
{
	//Clip the line before drawing it
	if (!ClipLine(line))
	{
		//the line is not in bounds, don't bother drawing it
		return;
	}

	//DDA line drawing algorithm
	float m = line.GetSlope();
	
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
			DrawPoint(ZPoint(x, y), color);
			yTrue = yTrue + m;
			y = round(yTrue);
			x++;
		}
	}
	else
	{
		//use y
		int y, yTop, x;
		float xTrue;

		m = 1.0 / m;

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
			DrawPoint(ZPoint(x, y), color);
			xTrue = xTrue + m;
			x = round(xTrue);
			y++;
		}
	}
}

bitset<4> XPMOutput::GetCohenSutherlandOutcode(ZPoint point)
{
	//point is in any several quadrants defined by bitwise arithmetic
	bitset<4> code = 0;
	code[0] = (point.x < lowerBound.x); //off left?
	code[1] = (point.x > upperBound.x); //off right?
	code[2] = (point.y < lowerBound.y); //off bottom?
	code[3] = (point.y > upperBound.y); //off top?
	return code;
}

//Clip the point passed in so it's now within bounds
//Move x & y so that they're in bounds, and move the other value so it matches with slope
ZPoint XPMOutput::ClipPoint(ZPoint point, ZLine line, int onlyEdge)
{
	ZPoint newPoint = point;
	float dx = line.endPoint.x - line.startPoint.x;
	if (point.x < lowerBound.x && (onlyEdge == -1 || onlyEdge == 0))
	{
		//left
		if (dx != 0)
		{
			newPoint.y = line.startPoint.y + (line.endPoint.y - line.startPoint.y)
				* (lowerBound.x - line.startPoint.x) / (line.endPoint.x - line.startPoint.x);
		}
		newPoint.x = lowerBound.x;
	}
	else if (point.x > upperBound.x && (onlyEdge == -1 || onlyEdge == 1))
	{
		//right
		if (dx != 0)
		{
			newPoint.y = line.startPoint.y + (line.endPoint.y - line.startPoint.y)
				* (upperBound.x - line.startPoint.x) / (line.endPoint.x - line.startPoint.x);
		}
		newPoint.x = upperBound.x;
	}
	float dy = line.endPoint.y - line.startPoint.y;
	if (newPoint.y < lowerBound.y && (onlyEdge == -1 || onlyEdge == 2))
	{
		//bottom
		if (dy != 0)
		{
			newPoint.x = line.startPoint.x + (line.endPoint.x - line.startPoint.x)
				* (lowerBound.y - line.startPoint.y) / (line.endPoint.y - line.startPoint.y);
		}
		newPoint.y = lowerBound.y;
	}
	else if (newPoint.y > upperBound.y && (onlyEdge == -1 || onlyEdge == 3))
	{
		//top
		if (dy != 0)
		{
			newPoint.x = line.startPoint.x + (line.endPoint.x - line.startPoint.x)
				* (upperBound.y - line.startPoint.y) / (line.endPoint.y - line.startPoint.y);
		}
		newPoint.y = upperBound.y;
	}

	return newPoint;
}

///Edits the line passed in to fit within the bounds
///Returns if the line fits within the bounding box at all (ie false if the line is completely outside the bounds)
///Following Cohen Sutherland pseudocode from here: http://www.cc.gatech.edu/grads/h/Hao-wei.Hsieh/Haowei.Hsieh/code1.html
bool XPMOutput::ClipLine(ZLine& line)
{
	//the sketchy while true loop handles the case of both points being out of the box, but in different quadrants
	//should only take one or two loops before we'll return from both in or both out
	while (true)
	{
		bitset<4> startCode, startCopy, endCode, endCopy;
		startCopy = startCode = GetCohenSutherlandOutcode(line.startPoint);
		endCopy = endCode = GetCohenSutherlandOutcode(line.endPoint);
		if (startCode == bitset<4>(0) && endCode == bitset<4>(0))
		{
			//trivially, both points are within box
			return true;
		}
		else if ((startCopy &= endCopy) != bitset<4>(0))
		{
			//both points are outside both
			//trivially return
			return false;
		}
		else
		{
			//we actually have to do work
			bitset<4> code;
			ZPoint point;
			if (startCode != bitset<4>(0))
			{
				line.startPoint = ClipPoint(line.startPoint, line);
			}
			if (endCode != bitset<4>(0))
			{
				line.endPoint = ClipPoint(line.endPoint, line);
			}
			//go back through the while loop to check if it's in or out of bounds
		}
	}
}

//Implement Sutherland - Hodgeman algorithm
ZPolygon * XPMOutput::ClipPolygon(ZPolygon polygon)
{
	if (polygon.points.size() < 2)
	{
		//this polygon is too small, not even a line
		return new ZPolygon();
	}

	//Set up in/out info for each point
	vector<bitset<4>> pointsAreIn;
	for (int i = 0; i < polygon.points.size(); i++)
	{
		pointsAreIn.push_back(GetCohenSutherlandOutcode(polygon.points[i]));
	}
	pointsAreIn.resize(pointsAreIn.size() * 3);

	vector<ZLine> lines;
	ZLine hLine = ZLine(ZPoint(lowerBound.x, 0), ZPoint(upperBound.x, 0));
	lines.push_back(hLine);

	ZPolygon* newPolygon;
	ZPolygon* prevPolygon = &polygon;

	//go through each edge
	for (int edge = 0; edge < 4; edge++)
	{
		newPolygon = new ZPolygon();

		int pSize = prevPolygon->points.size();
		for (int i = 0; i <pSize; i++)
		{
			//Process based on whether this point and adjacent points are inside
			ZPoint * prevPoint;
			ZPoint * curPoint;
			int prevLocation;
			if (i == 0)
			{
				//first point wraps around to last
				prevLocation = pSize - 1;
			}
			else
			{
				prevLocation = i - 1;
			}
			prevPoint = &prevPolygon->points[prevLocation];
			curPoint = &prevPolygon->points[i];

			//handle cases for all the edges
			if (pointsAreIn[i][edge] == false)
			{
				//this point is in
				if (pointsAreIn[prevLocation][edge] != false)
				{
					//previous point not in... need to find intersection point
					ZPoint * newPoint = new ZPoint( ClipPoint(*prevPoint, ZLine(*prevPoint, *curPoint),edge) );
					newPolygon->AddPoint(*newPoint);
				}
				newPolygon->AddPoint(*curPoint);
			}
			else
			{
				//this point is out
				if (pointsAreIn[prevLocation][edge] == false)
				{
					//previous point in.. need to find interesection point
					ZPoint * newPoint =  new ZPoint( ClipPoint(*curPoint, ZLine(*prevPoint, *curPoint),edge) );
					newPolygon->AddPoint(*newPoint);
				}
				//else do nothing, both points are out
			}	
		}
		prevPolygon = newPolygon;
		//the polygon rotated so we have to redo the codes... means this is really inefficient
		for (int i = 0; i < prevPolygon->points.size(); i++)
		{
			pointsAreIn[i] = GetCohenSutherlandOutcode(prevPolygon->points[i]);
		}
	}
	return newPolygon;
}

void XPMOutput::DrawPolygon(ZPolygon & polygon, Color color)
{
	if (polygon.points.size() < 2)
	{
		return;
	}
	ZPolygon * clippedPoly = ClipPolygon(polygon);
	ZPoint * prev = &clippedPoly->points[clippedPoly->points.size()-1];
	for (int i = 0; i < clippedPoly->points.size(); i++)
	{
		DrawLine(ZLine(clippedPoly->points[i], *prev), color);
		prev = &clippedPoly->points[i];
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
