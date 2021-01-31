#pragma once
#include <d3d11.h>
#include <vector>
#include <GenericShader.h>


class Material
{
public:
	Material();
	void StartMaterialPass(ID3D11DeviceContext*);
	void EndMaterialPass(ID3D11DeviceContext*);
protected:

	ID3D11InputLayout* mInputLayout;
	VertexShader* mVertexShader;
	HullShader* mHullShader;
	DomainShader* mDomainShader;
	GeometryShader* mGeometryShader;
	//Stream Output Stage implemented elsewhere
	ID3D11RasterizerState* mRasterizerState;
	PixelShader* mPixelShader;
	//Output Merger Stage implemented elsewhere


};