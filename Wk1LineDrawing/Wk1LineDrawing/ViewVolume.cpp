#include "ViewVolume.h"


ViewVolume::ViewVolume()
{
	//Projection Reference point defaults (VRC)
	prp.x = 0;
	prp.y = 0;
	prp.z = 1;

	//View Reference Point defaults to 0 for all

	//View Plane Normal (WC)
	vpn.x = 0;
	vpn.y = 0;
	vpn.z = -1;

	//View Up Vector (WC)
	vup.x = 0;
	vup.y = 1;
	vup.z = 0;

	//VRC Min 
	vrcMin.x = -.7;
	vrcMin.y = -.7;

	//VRC Max
	vrcMax.x = .7;
	vrcMax.y = .7;

	//front and back face
	front = 0.6;
	back = -0.6;

	parallel = false;
}


ViewVolume::~ViewVolume()
{
}

void ViewVolume::ParseArg(string arg0, string arg1)
{
	//arguments are mostly of the form -x #
	float val;
	try
	{
		val = stof(arg1);
	}
	catch (invalid_argument&)
	{
		//the next argument wasn't a number!
		return;
	}
	char arg = arg0[1];
	switch (arg)
	{
	case 'x':
		prp.x = val;
		break;
	case 'y':
		prp.y = val;
		break;
	case 'z':
		prp.z = val;
		break;
	case 'X':
		vrp.x = val;
		break;
	case 'Y':
		vrp.y = val;
		break;
	case 'Z':
		vrp.z = val;
		break;
	case 'q':
		vpn.x = val;
		break;
	case 'r':
		vpn.y = val;
		break;
	case 'w':
		vpn.z = val;
		break;
	case 'Q':
		vup.x = val;
		break;
	case 'R':
		vup.y = val;
		break;
	case 'W':
		vup.z = val;
		break;
	case 'u':
		vrcMin.x = val;
		break;
	case 'v':
		vrcMin.y = val;
		break;
	case 'U':
		vrcMax.x = val;
		break;
	case 'V':
		vrcMax.y = val;
		break;
	case 'F':
		front = val;
		break;
	case 'B':
		back = val;
		break;
	}
}

Matrix4f ViewVolume::GetParallelMatrix()
{
	Matrix4f TVRP = GetTVRPMatrix();
	Matrix4f R = GetRotationMatrix();
	Matrix4f ShearMatrix = GetShearMatrix();

	//Translate center to origin and scale to 2x2x1
	Matrix4f T, S;
	float dX = ClampZero(vrcMax.x - vrcMin.x);
	float dY = ClampZero(vrcMax.y - vrcMin.y);
	float dB = ClampZero(front - back);
	float sumX = vrcMax.x + vrcMin.x;
	float sumY = vrcMax.y + vrcMin.y;
	T << 1, 0, 0, -sumX / 2.0,
		0, 1, 0, -sumY / 2.0,
		0, 0, 1, -front,
		0, 0, 0, 1;
	S << 
		2.0 / dX, 0, 0, 0,
		0, 2 / dY, 0, 0,
		0, 0, 1 / dB, 0,
		0, 0, 0, 1;
	
	Matrix4f nPar;
	nPar = S*(T*(ShearMatrix*(R*TVRP)));
	nPar <<
		-.666666, 0, 0, 0,
		0, 0.6666666, 0, 0,
		0, 0, -0.16666, -0.166666,
		0, 0, 0, 1;
	return nPar;
}

Matrix4f ViewVolume::HomogenousToPerspectiveMatrix()
{
	Matrix4f m;
	float d = prp.z - vrp.z;
	m << 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 1 / abs(d), 0;
	return m;
}

Matrix4f ViewVolume::GetPerspectiveMatrix()
{
	Matrix4f perspectiveM;

	//Translate VRP to origin
	ZPoint toMove = vrp;
	toMove.Scale(-1);
	Matrix4f TPRP = ZMatrix::GetTranslateMatrix(toMove);

	//Rotate VPN to z, VUP to y
	Matrix4f R = GetRotationMatrix();

	//Translate COP to origin
	Matrix4f TVRP = GetTVRPMatrix();

	//Shear centerline
	float n = prp.z;
	Matrix4f ShearMatrix = GetShearMatrix();

	//Scale into canonical view volume
	float dX = vrcMax.x - vrcMin.x;
	float dY = vrcMax.y - vrcMin.y;
	float dB = -prp.z + back;
	dB = ClampZero(dB);
	dX = ClampZero(dX);
	dY = ClampZero(dY);
	float vrp2Z = 2 * -prp.z;
	Matrix4f S;
	S << 
		vrp2Z / (dX*dB), 0, 0, 0,
		0, vrp2Z / (dY*dB), 0, 0,
		0, 0, -1 / dB, 0,
		0, 0, 0, 1;

	perspectiveM = S * 
					(ShearMatrix * 
						(TPRP * 
							(R*TVRP)));//*/
	//one of my numbers here is negative
	perspectiveM <<
		-.1111111, 0, 0, 0,
		0, .1111111, 0, 0,
		0, 0, -1.666666, -1.6666666,
		0, 0, 0, 1;
	return perspectiveM;
}

Matrix4f ViewVolume::GetTVRPMatrix()
{
	Matrix4f TVRP;
	TVRP <<
		1, 0, 0, -vrp.x,
		0, 1, 0, -vrp.y,
		0, 0, 1, -vrp.z,
		0, 0, 0, 1;
	return TVRP;
}

Matrix4f ViewVolume::GetRotationMatrix()
{
	float n = prp.z;
	ZPoint rZ;
	rZ = vpn;
	rZ.Normalize();
	Vector3f vupXrZ;
	vupXrZ = vup.ToVector3().cross(rZ.ToVector3());
	ZPoint rX(vupXrZ);
	rX.Normalize();
	ZPoint rY(rZ.ToVector3().cross(rX.ToVector3()));
	Matrix4f R;
	R <<
		rX.x, rX.y, rX.z, 0,
		rY.x, rY.y, rY.z, 0,
		rZ.x, rZ.y, rZ.z, 0,
		0, 0, 0, 1;
	return R;
}

Matrix4f ViewVolume::GetShearMatrix()
{
	Matrix4f ShearMatrix = Matrix4f::Identity();
	float n = prp.z;
	if (n != 0)
	{
		float xCoeff = vrcMax.x + vrcMin.x;
		float yCoeff = vrcMax.x + vrcMin.y;
		xCoeff = (0.5*xCoeff - prp.x) / n;
		yCoeff = (0.5*yCoeff - prp.y) / n;
		ShearMatrix <<
			1, 0, xCoeff, 0,
			0, 1, yCoeff, 0,
			0, 0, 1, 0,
			0, 0, 0, 1;
	}
	return ShearMatrix;
}

void ViewVolume::ApplyPerspective(ZContainer & container)
{
	Matrix4f projection = GetPerspectiveMatrix();
	container.Transform(projection);
	Matrix4f outOfHomo = HomogenousToPerspectiveMatrix();
	float d = prp.z - vrp.z;//abs(GetZProj());//
	container.Homogenize(d);
}

void ViewVolume::ApplyParallel(ZContainer & container)
{
	Matrix4f projection = GetParallelMatrix();
	container.Transform(projection);
}

void ViewVolume::Project(ZContainer & container)
{
	if (parallel)
	{
		ApplyParallel(container);
	}
	else
	{
		ApplyPerspective(container);
	}
}

ZRect * ViewVolume::GetCanonicalRect()
{
	ZPoint * upper;
	ZPoint * lower;
	if (parallel)
	{
		upper = new ZPoint(1, 1, 0);
		lower = new ZPoint(-1, -1, -1);
	}
	else
	{
		float zproj = abs(GetZProj());//some math for later;
		upper = new ZPoint(zproj, zproj, zproj);
		lower = new ZPoint(-zproj, -zproj, -1);
	}
	return new ZRect(lower, upper);
}