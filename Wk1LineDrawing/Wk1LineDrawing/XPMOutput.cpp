#include "XPMOutput.h"

//ZH -- woo!  global variables.  really starting to hack 5-15
float xpmZmin = 1000;

float xpmZmax = -1000;

ColorWShade::ColorWShade(Color color, float z)
{
	baseColor = color;
	ZRect canonicalRect = *argsViewVolume.GetCanonicalRect();
	float minZ = xpmZmin;//canonicalRect.lowerBound.z;
	float maxZ = xpmZmax;//canonicalRect.upperBound.z;
	float diff = abs(maxZ - minZ);
	//distribute shade along z
	shade = abs(z-minZ) / diff * (float)maxShades;
}

//convert shade 0-20 to a value "00" to "ff"
	//so output "00"-"ff" depending on shade
string ColorWShade::ToHexString()
{
	//I don't think this worked!
	float maxColor = 255.0;
	float diffPerShade = 255.0 / ((float)maxShades+1.0); //+1 so darkest shade is not black
	float shade255 = ((float) shade +1.0)* diffPerShade;

	//convert to hex
	stringstream stream;
	stream << hex << (int)shade255;
	
	//space hex code to 2 digits
	string hexString = stream.str();
	if (hexString.length() == 1)
	{
		//apparently this erases/clears the stream
		stream.str("");
		stream << "0" << hexString;
		hexString = stream.str();
	}

	return hexString;
}

//output shade for reference in XPM output
	//format C hex
	//ex: R12	//for a red color with shade 12
string ColorWShade::ToXPMString()
{
	if (baseColor == Color::BLACK)
	{
		//black has no shades
		return " ";
	}
	//Combine shade & color into an ASCII value
	int startPos = 48;
	if (baseColor == Color::RED)
	{
		startPos += 0;
	}
	else if (baseColor == Color::GREEN)
	{
		startPos += 20;
	}
	else if (baseColor == Color::BLUE)
	{
		//blue doesn't work on my computer but maybe it works elsewhere
		startPos = 'f';
	}
	char c = (startPos + shade);
	switch (c)
	{
	/*case ';':
		c = 112;
		break;
	case '?':
		c = 113;
		break;
	case '@':
		c = 114;
		break;*/
	default:
		break;
	}

	stringstream stream;
	stream << c;
	return stream.str();
}

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
			grid[i][j].c = ColorWShade(Color::BLACK);
			grid[i][j].p = *(new ZPoint());
		}
	}
}


XPMOutput::~XPMOutput()
{
}

//output 20 different shades for the color
string ColorDeclaration(Color color)
{
	stringstream stream;
	ColorWShade shadeRef;
	for (int i = 0; i < shadeRef.maxShades; i++)
	{
		ColorWShade thisShade(color);
		thisShade.shade = i;
		string hexS = thisShade.ToHexString();
		
		//determine positioning of hex in string
		stringstream colorSS;
		if (color == Color::RED)
		{
			colorSS << hexS << "0000";
		}
		else if (color == Color::GREEN)
		{
			colorSS << "00" << hexS << "00";
		}
		else if (color == Color::BLUE)
		{
			colorSS << "0000" << hexS;
		}
		else
		{
			//color is black
			colorSS << "000000";
		}

		//actually output!
		stream << "\"" << thisShade.ToXPMString() << " c #" << colorSS.str() << "\"," << endl;
	}

	return stream.str();
}

//Assignment 5 started adding drawing with shades
void XPMOutput::Output(ostream* out)
{
	const string startingLines = "/* XPM */ \n static char *sco100[] = { \n \n /* width height num_colors chars_per_pixel */ \n";
	*out << startingLines << endl;
	int width = bounds.GetWidth()+1;
	int height = bounds.GetHeight()+1;
	*out << "\"" << width << " " << height << " " << num_colors << " " << chars_per_pixel << "\"," << endl;
	*out << "/* colors */" << endl;
	*out << "\"" << ColorWShade(Color::BLACK).ToXPMString() << " c #ffffff\"," << endl;
	
	//declare 3 primary colors
	*out << ColorDeclaration(Color::RED);
	*out << ColorDeclaration(Color::GREEN);
	*out << ColorDeclaration(Color::BLUE);

	*out << "/* pixels */" << endl;
	for (int y = height-1; y >=0; y--)
	{
		*out << "\"";
		for (int x = 0; x < width; x++)
		{
			*out << grid[x][y].c.ToXPMString();
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
		grid[point.x][point.y].c = ColorWShade(color, point.z);
		grid[point.x][point.y].p = point;
	}
}


///Draw a line onto pixels
///While loop algorithm pseudo code from here: http://ocw.unican.es/ensenanzas-tecnicas/visualizacion-e-interaccion-grafica/material-de-clase-2/03-LineAlgorithms.pdf
//ZH added z drawing with floats  for Assignemtn 5
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

		//find z slope
		float zXM = (maxPoint.z - minPoint.z) / (maxPoint.x - minPoint.x);
		float xMidPoint = (maxPoint.x + minPoint.x) / 2.0;

		while (x < xRight)
		{
			float z;
			z = minPoint.z + zXM * (float)(x - minPoint.x); //floating point slow
			if (x < xMidPoint)
			{
		//		z = minPoint.z;
			}
			else
			{
			//	z = maxPoint.z;
			}
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

		//find z slope
		float zYM = (maxPoint.z - minPoint.z) / (maxPoint.y - minPoint.y);
		float yMidPoint = (maxPoint.y + minPoint.y) / 2.0;

		while (y < yTop)
		{
			float z = minPoint.z + zYM * (y - minPoint.y); //floating point slow
			if (x < yMidPoint)
			{
				//z = minPoint.z;
			}
			else
			{
				//z = maxPoint.z;
			}
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

bool PointSorter(ZPoint i, ZPoint j) { return (i.x < j.x); };

void XPMOutput::FillPolygon(ZPolygon & polygon, Color color)
{
	if (polygon.points.size() <= 0)
	{
		//don't process non-existant polygons
		return;
	}

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
		vector<ZPoint> intersectionPoints;
		for (auto line : lines)
		{
			float curX;
			float reverseM = ((float)line.endPoint.x - (float)line.startPoint.x) / ((float)line.endPoint.y - (float)line.startPoint.y);
			curX = floor((float)line.startPoint.x + reverseM* (float)(curY - (float)line.startPoint.y));

			//ZH 3-15 copied above curX code, not sure what did.  Generally refactored code to take points rather than just xs
			float curZ;
			reverseM = ((float)line.endPoint.z - (float)line.startPoint.z) / ((float)line.endPoint.y - (float)line.startPoint.y);
			curZ = (float)line.startPoint.z + reverseM* (float)(curY - (float)line.startPoint.y);

			ZPoint * p = new ZPoint(curX, curY, curZ);
			intersectionPoints.push_back(*p);
		}


		//sort the points by xs
		sort(intersectionPoints.begin(), intersectionPoints.end(),PointSorter);
		ZPoint * prevPoint = NULL;
		for (auto p : intersectionPoints)
		{
			if (prevPoint == NULL)
			{
				prevPoint = new ZPoint(p);
			}
			else
			{
				//handle duplicates???????
				ZLine toDraw = ZLine(*prevPoint, p);
				DrawLine(toDraw, color);
				prevPoint = NULL;
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

	//find z max & min for cheap shading good nesss hack ZH 3-15z
	float zMin = 1000;
	float zMax = -1000;
	for (int i = 0; i < image.polygons.size(); i++)
	{
		ZPolygon  * poly = &image.polygons[i];
		for (int j = 0; j < poly->points.size(); j++)
		{
			float curZ = poly->points[j].z;
			if (curZ < zMin)
			{
				zMin = curZ;
			}
			if (curZ > zMax)
			{
				zMax = curZ;
			}
		}
	}
	xpmZmin = zMin;
	xpmZmax = zMax;

	for (int i = 0; i < image.polygons.size(); i++)
	{
		FillPolygon(image.polygons[i], color);
	}
}

