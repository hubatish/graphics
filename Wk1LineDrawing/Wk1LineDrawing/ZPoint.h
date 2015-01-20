#ifndef Z_POINT_H
#define Z_POINT_H
#include <vector>
#pragma once
using namespace std;

class ZPoint
{
public:
	ZPoint(int x, int y);
	ZPoint();
	~ZPoint();

	void Translate(ZPoint point) { x += point.x; y += point.y; }
	void Rotate(float degreeAngle);
	int x, y;
};

class ZLine
{
public:
	ZLine(ZPoint start, ZPoint end);
	ZLine();
	~ZLine();

	float GetSlope();
	ZPoint startPoint, endPoint;
};

class ZImage
{
public:
	ZImage();
	~ZImage();

	void AddLine(ZLine);

	vector<ZLine> lines;

	void Scale(float f);
	void Translate(ZPoint point);
	void Rotate(float angle);
	void Clip(ZPoint lower, ZPoint upper);
};

#endif