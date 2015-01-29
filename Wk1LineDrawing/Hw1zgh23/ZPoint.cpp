#include "ZPoint.h"


ZPoint::ZPoint(int x, int y)
{
	this->x = x;
	this->y = y;
}

ZPoint::ZPoint()
{
	x = 0;
	y = 0;
}

ZPoint::~ZPoint()
{
}

float pi = atan(1)*4;

void ZPoint::Rotate(float degreeAngle)
{
	float angle = degreeAngle / 180.0 * pi;
	int xNew = x * cos(angle) - y * sin(angle);
	y = x * sin(angle) + y * cos(angle);
	x = xNew;
}

ZLine::ZLine(ZPoint start, ZPoint end)
{
	this->startPoint = start;
	this->endPoint = end;
}

ZLine::ZLine()
{
	startPoint = ZPoint();
	endPoint = ZPoint();
}

ZLine::~ZLine()
{
}

float ZLine::GetSlope()
{
	//returns slope, with bad compromise of returning max int in case of infinite slope (could maybe return INFINITY instead)
	float dX = endPoint.x - startPoint.x;
	if (dX == 0)
	{
		return INT_MAX;
	}
	else
	{
		return ((float)endPoint.y - (float)startPoint.y) / dX;
	}
}

ZImage::ZImage()
{
}

ZImage::~ZImage()
{
}

void ZImage::AddLine(ZLine line)
{
	lines.push_back(line);
}

void ZImage::Scale(float f)
{
	for (int i = 0; i < lines.size(); i++)
	{
		lines[i].startPoint.x *= f;
		lines[i].startPoint.y *= f;
		lines[i].endPoint.y *= f;
		lines[i].endPoint.x *= f;
	}
}

void ZImage::Translate(ZPoint point)
{
	for (int i = 0; i < lines.size(); i++)
	{
		lines[i].startPoint.Translate(point);
		lines[i].endPoint.Translate(point);
	}
}

//Rotate counterclockwise about the origin
void ZImage::Rotate(float angle)
{
	for (int i = 0; i < lines.size(); i++)
	{
		lines[i].startPoint.Rotate(angle);
		lines[i].endPoint.Rotate(angle);
	}
}
