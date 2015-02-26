#include "ZMatrix.h"


ZMatrix::ZMatrix()
{
}


ZMatrix::~ZMatrix()
{
}

Matrix4f ZMatrix::GetScaleMatrix(ZPoint scale)
{
	Matrix4f * m = new Matrix4f(Matrix4f::Zero());
	(*m)(0, 0) = scale.x;
	(*m)(1, 1) = scale.y;
	(*m)(2, 2) = scale.z;
	(*m)(3, 3) = 1;
	return *m;
}

Matrix4f ZMatrix::GetScaleTranslate(ZPoint toMove)
{
	Matrix4f * m = new Matrix4f();
	(*m)(0, 0) = 1;
	(*m)(0, 4) = toMove.x;
	(*m)(1, 1) = 1;
	(*m)(1, 4) = toMove.y;
	(*m)(2, 2) = 1;
	(*m)(2, 4) = toMove.z;
	(*m)(3, 3) = 1;
	return *m;
}
