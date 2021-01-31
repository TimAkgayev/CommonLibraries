#pragma once
#include <DirectXMath.h>
#include <Line.h>
using namespace DirectX;

class Joint
{
public:
	
	XMFLOAT3 position;

	XMVECTOR x_basis;
	XMVECTOR y_basis;
	XMVECTOR z_basis;

	Joint* parentJoint;
	Joint* childJoint;
};

