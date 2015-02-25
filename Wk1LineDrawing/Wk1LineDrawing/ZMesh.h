#pragma once
#include "ZPoint.h"

using namespace std;

class ZMesh
{
public:
	ZMesh();
	ZMesh(const ZMesh & image);
	~ZMesh();

	void AddPolygon(ZPolygon polygon);

	vector<ZPolygon> polygons;

	void Scale(float f);
	void Scale(float xScale, float yScale);
	void Translate(ZPoint point);
	void Rotate(float angle);
	void Clip(ZPoint lower, ZPoint upper);
};
