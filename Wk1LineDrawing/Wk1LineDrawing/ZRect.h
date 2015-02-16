#ifndef Z_RECT_H
#define Z_RECT_H

#pragma once
#include "ZPoint.h"
#include <bitset>

using namespace std;

class ZRect
{
private:
	bitset<4> GetCohenSutherlandOutcode(ZPoint point);

public:
	//Requirements for bounds: lowerBoundx<=upperBoundx & lowerBoundy<=upperBoundy
	ZPoint lowerBound;
	ZPoint upperBound;

	ZRect(ZPoint* lowerBound = NULL, ZPoint* upperBound = NULL);
	~ZRect();

	float GetWidth(){ return (upperBound.x - lowerBound.x + 1.0); }
	float GetHeight() { return (upperBound.y - lowerBound.y + 1.0); }

	//help with line clipping algorithm
	bool ClipLine(ZLine & line);
	ZPolygon* ClipPolygon(ZPolygon polygon);
	ZPoint ClipPoint(ZPoint point, ZLine line, int onlyEdge = -1);
	void ClipImage(ZImage & image);
};

class BoundedImage
{
public:
	BoundedImage(ZImage * image, ZRect * rect = NULL);
	~BoundedImage();

	ZRect bounds;
	ZImage image;

	void ClipImage(){ bounds.ClipImage(image); };

	BoundedImage * FitToViewort(ZRect & view);
};

#endif