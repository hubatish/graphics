#include "ZPoint.h"

ZContainer::ZContainer(const ZContainer & c)
{
}

ZPoint::ZPoint(float x, float y)
{
	this->x = x;
	this->y = y;
	this->z = 0;
}

ZPoint::ZPoint(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

ZPoint::ZPoint(const ZPoint & p)
{
	x = p.x;
	y = p.y;
	z = p.z;
}

ZPoint::ZPoint(Vector3f v)
{
	x = v(0);
	y = v(1);
	z = v(2);
}

ZPoint::ZPoint()
{
	x = 0;
	y = 0;
	z = 0;
}

ZPoint::~ZPoint()
{
}

float pi = atan(1)*4;

void ZPoint::Rotate(float degreeAngle)
{
	float angle = degreeAngle / 180.0 * pi;
	int xNew = x * cos(angle) - y * sin(angle);
	y = x * sin(angle) + y * cos(angle);
	x = xNew;
}

void ZPoint::Translate(ZPoint point)
{
	Matrix4f m = ZMatrix::GetTranslateMatrix(point);
	Transform(m);
//	x += point.x; y += point.y;
}

void ZPoint::Scale(float xScale, float yScale)
{ 
	Matrix4f m = ZMatrix::GetScaleMatrix(ZPoint(xScale, yScale, 1));
	Transform(m);
	//x *= xScale; y *= yScale; 
}

void ZPoint::Transform(const Matrix4f & m)
{
	Vector4f v;
	v << x,
		y,
		z,
		1;
	v = m * v;
	x = v(0);
	y = v(1);
	z = v(2);
}

void ZPoint::Normalize()
{
	Scale(1.0 / GetMagnitude());
}

float ZPoint::GetMagnitude()
{
	float sum = x*x + y*y + z*z;
	return sqrt(sum);
}

Vector3f ZPoint::ToVector3()
{
	Vector3f v;
	v << x,
		y,
		z;
	return v;
}

ZLine::ZLine(ZPoint start, ZPoint end)
{
	this->startPoint = start;
	this->endPoint = end;
}

ZLine::ZLine(const ZLine & l)
{
	startPoint = ZPoint(l.startPoint);
	endPoint = ZPoint(l.endPoint);
}

ZLine::ZLine()
{
	startPoint = ZPoint();
	endPoint = ZPoint();
}

ZLine::~ZLine()
{
}

void ZLine::Translate(ZPoint point)
{
	startPoint.Translate(point);
	endPoint.Translate(point);
}

void ZLine::Scale(float scaleFactor)
{
	startPoint.Scale(scaleFactor);
	endPoint.Scale(scaleFactor);
}

void ZLine::Scale(float xScale, float yScale)
{
	startPoint.Scale(xScale,yScale);
	endPoint.Scale(xScale, yScale);
}

void ZLine::Rotate(float angle)
{
	startPoint.Rotate(angle);
	endPoint.Rotate(angle);
}

void ZLine::Transform(const Matrix4f & m)
{
	startPoint.Transform(m);
	endPoint.Transform(m);
}

float ZLine::GetSlope()
{
	//returns slope, with bad compromise of returning max int in case of infinite slope (could maybe return INFINITY instead)
	float dX = endPoint.x - startPoint.x;
	if (dX == 0)
	{
		return INT_MAX;
	}
	else
	{
		return ((float)endPoint.y - (float)startPoint.y) / dX;
	}
}

ZPolygon::ZPolygon()
{
	//points = new vector<ZPoint>();
}

ZPolygon::ZPolygon(const ZPolygon & p)
{
	points = p.points;
}

ZPolygon::~ZPolygon()
{
}

void ZPolygon::AddPoint(ZPoint point)
{
	points.push_back(point);
}

void ZPolygon::Translate(ZPoint point)
{
	for (int i = 0; i < points.size(); i++)
	{
		points[i].Translate(point);
	}
}

void ZPolygon::Scale(float scaleFactor)
{
	for (int i = 0; i < points.size(); i++)
	{
		points[i].Scale(scaleFactor);
	}
}

void ZPolygon::Scale(float xScale, float yScale)
{
	for (int i = 0; i < points.size(); i++)
	{
		points[i].Scale(xScale, yScale);
	}
}

void ZPolygon::Rotate(float angle)
{
	for (int i = 0; i < points.size(); i++)
	{
		points[i].Rotate(angle);
	}
}

void ZPolygon::Transform(const Matrix4f & m)
{
	for (int i = 0; i < points.size();i++)
	{
		points[i].Transform(m);
	}
}

//Assuming the polygon is a coplanar triangle, 
	//Return the vector normal to that plane
ZPoint ZPolygon::FindNormal()
{
	if (points.size() < 3)
	{
		return *(new ZPoint());
	}
	ZPoint* p;
	//Assumes the polygon is a triangle
	Vector3f p1 = points[1].ToVector3();
	Vector3f p2 = points[2].ToVector3();
	Vector3f p0 = points[0].ToVector3();
	Vector3f n = (p1 - p0).cross(p2 - p0);
	p = new ZPoint(n);
	p->Normalize();
	return *p;
}

void ZPolygon::ApplyFunction(void(*foo) (ZPoint &, ZPoint), ZPoint arg)
{
	for (int i = 0; i < points.size(); i++)
	{
		foo(points[i], arg);
	}
}

ZImage::ZImage()
{
}

ZImage::ZImage(const ZImage & image)
{
	lines = image.lines;
	polygons = image.polygons;
}

ZImage::~ZImage()
{
}

void ZImage::AddPolygon(ZPolygon polygon)
{
	polygons.push_back(polygon);
}

void ZImage::AddLine(ZLine line)
{
	lines.push_back(line);
}

void ZImage::Scale(float f)
{
	for (int i = 0; i < lines.size(); i++)
	{
		lines[i].Scale(f);
	}
	for (int i = 0; i < polygons.size(); i++)
	{
		polygons[i].Scale(f);
	}
}

void ZImage::Scale(float xScale, float yScale)
{
	for (int i = 0; i < lines.size(); i++)
	{
		lines[i].Scale(xScale, yScale);
	}
	for (int i = 0; i < polygons.size(); i++)
	{
		polygons[i].Scale(xScale, yScale);
	}
}

void ZImage::Translate(ZPoint point)
{
	for (int i = 0; i < lines.size(); i++)
	{
		lines[i].Translate(point);
	}
	for (int i = 0; i < polygons.size(); i++)
	{
		polygons[i].Translate(point);
	}
}

//Rotate counterclockwise about the origin
void ZImage::Rotate(float angle)
{
	for (int i = 0; i < lines.size(); i++)
	{
		lines[i].Rotate(angle);
	}
	for (int i = 0; i < polygons.size(); i++)
	{
		polygons[i].Rotate(angle);
	}
}

void ZImage::Transform(const Matrix4f & m)
{
	for (int i = 0; i < lines.size(); i++)
	{
		lines[i].Transform(m);
	}
	for (int i = 0; i < polygons.size(); i++)
	{
		polygons[i].Transform(m);
	}
}

void ZImage::Homogenize(float d)
{
	for (int i = 0; i < polygons.size(); i++)
	{
		for (int j = 0; j < polygons[i].points.size(); j++)
		{
			float zD = polygons[i].points[j].z / d;
			polygons[i].points[j].x /= zD;
			polygons[i].points[j].y /= zD;
			polygons[i].points[j].z = d;
		}
	}
}

void ZImage::CullBackFaces()
{
	for (int i = polygons.size() - 1; i >= 0; i--)
	{
		ZPoint n = polygons[i].FindNormal();
		if (n.z < 0)
		{
			//This face is a back-face!
			//Cull it!
			polygons.erase(polygons.begin() + i);
		}
	}
}