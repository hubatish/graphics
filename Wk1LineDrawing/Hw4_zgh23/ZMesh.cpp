#include "ZMesh.h"


ZMesh::ZMesh()
{
}

ZMesh::ZMesh(const ZMesh & image)
{
	polygons = image.polygons;
}

ZMesh::~ZMesh()
{
}

void ZMesh::AddPolygon(ZPolygon polygon)
{
	polygons.push_back(polygon);
}

void ZMesh::Scale(float f)
{
	for (int i = 0; i < polygons.size(); i++)
	{
		polygons[i].Scale(f);
	}
}

void ZMesh::Scale(float xScale, float yScale)
{
	for (int i = 0; i < polygons.size(); i++)
	{
		polygons[i].Scale(xScale, yScale);
	}
}

void ZMesh::Translate(ZPoint point)
{
	for (int i = 0; i < polygons.size(); i++)
	{
		polygons[i].Translate(point);
	}
}

//Rotate counterclockwise about the origin
void ZMesh::Rotate(float angle)
{
	for (int i = 0; i < polygons.size(); i++)
	{
		polygons[i].Rotate(angle);
	}
}
