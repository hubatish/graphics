#ifndef Z_XPM_OUTPUT_H
#define Z_XPM_OUTPUT_H

#pragma once
#include <string>
#include "ZPoint.h"
#include "ZRect.h"
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
	ZPoint negLowerBound;
	ZRect bounds;
	//Requirements for grid: has positions for every coordinate within the bounds
	//Accessed by [x][y]
	vector<vector<Color> > grid;

public:

	XPMOutput(ZRect * rect = NULL);
	~XPMOutput();
	void Output(ostream *out);

	void DrawLine(ZLine line, Color color);
	void DrawPoint(ZPoint point, Color color);
	void DrawPolygon(ZPolygon & polygon, Color color);
	void DrawImage(ZImage & image, Color color);

	void OffsetPoint(ZPoint & point){ point.Translate(negLowerBound); }
};

#endif
