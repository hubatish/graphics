#ifndef Z_POINT_H
#define Z_POINT_H
#include <vector>
#include <cmath>
#include <cstdint>
#include <climits>
#pragma once
using namespace std;

class ZPoint
{
public:
	ZPoint(int x, int y);
	ZPoint();
	ZPoint(const ZPoint & p);
	~ZPoint();

	void Translate(ZPoint point) { x += point.x; y += point.y; }
	void Scale(float f) { x *= f; y *= f; }
	void Rotate(float degreeAngle);

	int x, y;
};

class ZLine
{
public:
	ZLine(ZPoint start, ZPoint end);
	ZLine();
	ZLine(const ZLine & l);
	~ZLine();

	float GetSlope();
	ZPoint startPoint, endPoint;
	void Translate(ZPoint point);
	void Scale(float scaleFactor);
	void Rotate(float angle);
};

class ZPolygon
{
public:
	ZPolygon();
	ZPolygon(const ZPolygon & p);
	~ZPolygon();

	vector<ZPoint> points;

	void AddPoint(ZPoint point);
	void Translate(ZPoint point);
	void Scale(float scaleFactor);
	void Rotate(float angle);
};

class ZImage
{
public:
	ZImage();
	ZImage(const ZImage & image);
	~ZImage();

	void AddLine(ZLine line);
	void AddPolygon(ZPolygon polygon);

	vector<ZLine> lines;
	vector<ZPolygon> polygons;

	void Scale(float f);
	void Translate(ZPoint point);
	void Rotate(float angle);
	void Clip(ZPoint lower, ZPoint upper);
};

#endif
