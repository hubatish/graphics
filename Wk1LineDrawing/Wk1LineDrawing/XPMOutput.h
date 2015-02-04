#ifndef Z_XPM_OUTPUT_H
#define Z_XPM_OUTPUT_H

#pragma once
#include <string>
#include "ZPoint.h"
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <cmath>
#include <bitset>
using namespace std;

enum class Color : char { BLACK = 'B', WHITE = 'W' };

class XPMOutput
{
private:
	const static int num_colors = 2;
	const static int chars_per_pixel = 1;
	//Requirements for bounds: lowerBoundx<=upperBoundx & lowerBoundy<=upperBoundy
	ZPoint lowerBound, upperBound, negLowerBound;
	//Requirements for grid: has positions for every coordinate within the bounds
	//Accessed by [x][y]
	vector<vector<Color> > grid;
	bitset<4> GetCohenSutherlandOutcode(ZPoint point);

public:

	XPMOutput(ZPoint* lowerBound = NULL, ZPoint* upperBound = NULL);
	~XPMOutput();
	void Output(ostream *out);
	int GetWidth(){ return (upperBound.x - lowerBound.x); }
	int GetHeight() { return (upperBound.y - lowerBound.y); }

	void DrawLine(ZLine line, Color color);
	void DrawPoint(ZPoint point, Color color);
	void DrawPolygon(ZPolygon & polygon, Color color);
	void DrawImage(ZImage & image, Color color);

	//help with line clipping algorithm
	bool ClipLine(ZLine & line);
	ZPolygon* ClipPolygon(ZPolygon polygon);
	ZPoint ClipPoint(ZPoint point, ZLine line, int onlyEdge = -1);

	void OffsetPoint(ZPoint & point){ point.Translate(negLowerBound); }
};

#endif
