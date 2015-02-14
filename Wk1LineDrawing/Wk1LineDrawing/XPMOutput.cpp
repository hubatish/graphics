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
	int width = bounds.GetWidth();
	int height = bounds.GetHeight();
	
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
	int width = bounds.GetWidth();
	int height = bounds.GetHeight();
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
	if (!bounds.ClipLine(line))
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


void XPMOutput::DrawPolygon(ZPolygon & polygon, Color color)
{
	if (polygon.points.size() < 2)
	{
		return;
	}
	ZPolygon * clippedPoly = bounds.ClipPolygon(polygon);
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
