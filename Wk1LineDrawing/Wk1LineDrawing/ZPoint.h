#ifndef Z_POINT_H
#define Z_POINT_H
#include <vector>
#include <cmath>
#include <cstdint>
#include <climits>
#include <Eigen/Dense>
#include "ZMatrix.h"
#pragma once
using namespace std;
using namespace Eigen;

class ZPoint
{
public:
	ZPoint(float x, float y);
	ZPoint(float x, float y, float z);
	ZPoint();
	ZPoint(const ZPoint & p);
	~ZPoint();

	static void TranslateP(ZPoint & point, ZPoint translation){ point.Translate(translation); } //static for application experiment
	void Translate(ZPoint point);// { x += point.x; y += point.y; }
	void Scale(float f) { x *= f; y *= f; }
	void Scale(float xScale, float yScale);
	void Rotate(float degreeAngle);

	void Transform(const Matrix4f & m);

	float x, y, z;
};

class ZContainer
{
public:
	ZContainer(){}
	ZContainer(const ZContainer & c);
	~ZContainer(){}

	virtual void Scale(float f){}
	virtual void Scale(float xScale, float yScale){}
	virtual void Translate(ZPoint point){}
	virtual void Rotate(float angle){}
};

class ZLine : public ZContainer
{
public:
	ZLine(ZPoint start, ZPoint end);
	ZLine();
	ZLine(const ZLine & l);
	~ZLine();

	float GetSlope();
	static bool IsSlopeVertical(float m)
	{
		return(m == INT_MAX);
	}
	ZPoint startPoint, endPoint;
	void Translate(ZPoint point);
	void Scale(float scaleFactor);
	void Scale(float xScale, float yScale);
	void Rotate(float angle);
};

class ZPolygon : public ZContainer
{
public:
	ZPolygon();
	ZPolygon(const ZPolygon & p);
	~ZPolygon();

	vector<ZPoint> points;

	void ApplyFunction(void(*foo) (ZPoint &, ZPoint), ZPoint arg);
	void AddPoint(ZPoint point);
	void Translate(ZPoint point);
	void Scale(float scaleFactor);
	void Scale(float xScale, float yScale);
	void Rotate(float angle);
};

class ZImage : public ZContainer
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
	void Scale(float xScale, float yScale);
	void Translate(ZPoint point);
	void Rotate(float angle);
	void Clip(ZPoint lower, ZPoint upper);
};

#endif
