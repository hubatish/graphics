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

Matrix4f ViewVolume::GetPerspectiveMatrix()
{
	Matrix4f perspectiveM;

	//Translate VRP to origin
	ZPoint toMove = vrp;
	toMove.Scale(-1);
	Matrix4f TPRP = ZMatrix::GetTranslateMatrix(toMove);

	//Rotate VPN to z, VUP to y
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
		rX.x, rX.y, rX.z,0,
		rY.x, rY.y, rY.z,0,
		rZ.x, rZ.y, rZ.z,0,
		0 , 0 , 0 , 1;

	//Translate COP to origin
	Matrix4f TVRP;
	TVRP << 
		1, 0, 0, -prp.x,
		0, 1, 0, -prp.y,
		0, 0, 1, -prp.z,
		0, 0, 0, 1;

	//Shear centerline
	float n = prp.z;
	Matrix4f ShearMatrix = Matrix4f::Zero();

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

	//Scale into canonical view volume
	float dX = vrcMax.x - vrcMin.x;
	float dY = vrcMax.y - vrcMin.y;
	float dB = -prp.z + back;
	if (dB == 0)
	{
		dB = 0.01;
	}
	if (dX == 0)
	{
		dX = 0.01;
	}
	if (dY == 0)
	{
		dY = 0.01;
	}
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
	return perspectiveM;
}

