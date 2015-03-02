#pragma once
#include "ZPoint.h"
#include "ZMatrix.h"
#include <Eigen/Dense>
#include "ZRect.h"

using namespace Eigen;

class ViewVolume
{
private:
	Matrix4f GetTVRPMatrix();
	Matrix4f GetRotationMatrix();
	Matrix4f GetShearMatrix();
	float ClampZero(float f)
	{
		if (f == 0)
		{
			f = 0.01;
		}
		return f;
	}

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
	//Perspective or parallel volume?
	bool parallel;

	float GetZProj()
	{
		float dB = ClampZero(back - prp.z);
		return abs(prp.z / dB);
	}

	ViewVolume();
	~ViewVolume();

	void ParseArg(string arg0, string arg1);

	ZRect * GetCanonicalRect();

	Matrix4f GetParallelMatrix();
	Matrix4f GetPerspectiveMatrix();
	void ApplyPerspective(ZContainer & container);
	void ApplyParallel(ZContainer & container);
	void Project(ZContainer & container);
};

