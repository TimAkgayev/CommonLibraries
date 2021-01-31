#pragma once
#include <d3d11.h>
#include <ObjFileParser.h>
#include <SpacePartitioning.h>


class DXAModel
{
public:
	DXAModel();

	virtual void Draw(ID3D11DeviceContext*) = 0;

protected:
	//Input Assembler 
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	D3D11_PRIMITIVE_TOPOLOGY mTopology;
	UINT mNumIndices;

	//Pixel Shader Stage
	ID3D11ShaderResourceView* mPSResourceView;
	ID3D11SamplerState* mPSSampler;
};

