#pragma once
#include <Eigen/Dense>
#include "ZPoint.h"
using namespace Eigen;

class ZPoint;

class ZMatrix
{
public:
	ZMatrix();
	~ZMatrix();

	static Matrix4f GetScaleMatrix(ZPoint scale);
	static Matrix4f GetTranslateMatrix(ZPoint toMove);
};

