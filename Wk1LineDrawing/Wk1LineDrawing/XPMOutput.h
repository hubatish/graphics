#ifndef Z_XPM_OUTPUT_H
#define Z_XPM_OUTPUT_H

#pragma once
#include <string>
#include "ZPoint.h"
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <math.h>
#include <bitset>
using namespace std;

class XPMOutput
{
public:
	enum class Color : char { BLACK = 'B', WHITE = 'W' };

	XPMOutput(ZPoint* lowerBound = NULL, ZPoint* upperBound = NULL);
	~XPMOutput();
	void Output(ostream *out);
	int GetWidth(){ return (upperBound.x - lowerBound.x); }
	int GetHeight() { return (upperBound.y - lowerBound.y); }

	void DrawLine(ZLine line, Color color);
	void DrawPoint(ZPoint point, Color color){ grid[point.x][point.y] = color; }
	void DrawImage(ZImage & image, Color color);

	bool ClipLine(ZLine & line);
	ZPoint ClipPoint(ZPoint point, ZLine line);

private:
	const static int num_colors = 2;
	const static int chars_per_pixel = 1;
	//Requirements for bounds: lowerBoundx<=upperBoundx & lowerBoundy<=upperBoundy
	ZPoint lowerBound, upperBound;
	//Requirements for grid: has positions for every coordinate within the bounds
	//Accessed by [x][y]
	vector<vector<Color>> grid;
	bitset<4> GetCohenSutherlandOutcode(ZPoint point);
};

#endif