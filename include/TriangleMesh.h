#pragma once
#include "DXAModel.h"

class TriangleMesh : public DXAModel
{
public:
	TriangleMesh(ID3D11Device* device, GenericObject LoadedObject);


	const BVH::node*	GetTree() const;
	virtual void		Draw(ID3D11DeviceContext* context) override;
	GenericObject&		GetObject();

protected:

	BVH::triangle* mTriangleList;
	UINT mNumTriangles;
	const BVH::node* mTree;
	GenericObject mObject;

};

