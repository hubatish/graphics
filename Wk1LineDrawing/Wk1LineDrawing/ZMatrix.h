#pragma once
#include <Eigen/Dense>
#include "ZPoint.h"
//#include <sstream>
using namespace Eigen;
using namespace std;

class ZPoint;

class ZMatrix
{
public:
	ZMatrix();
	~ZMatrix();

	static Matrix4f GetScaleMatrix(ZPoint scale);
	static Matrix4f GetTranslateMatrix(ZPoint toMove);
	static string GetString(const Matrix4f & m);
};

