#pragma once
#include "ZPoint.h"
#include "ZMatrix.h"
#include <Eigen/Dense>

using namespace Eigen;

class ViewVolume
{
public:
	//Projection reference point (VRCoordinates)
	ZPoint prp;
	//View Reference point
	ZPoint vrp;
	//View plane normal vvector
	ZPoint vpn;
	//View up vector
	ZPoint vup;
	//min (VRC)
	ZPoint vrcMin;
	//max (VRC)
	ZPoint vrcMax;
	//Front clipping plane
	float front;
	//Back clipping plane
	float back;

	ViewVolume();
	~ViewVolume();

	void ParseArg(string arg0, string arg1);

	Matrix4f GetPerspectiveMatrix();
};

