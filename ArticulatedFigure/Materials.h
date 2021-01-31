#pragma once
#include <Material.h>
#include <DirectXMath.h>
class LineMaterial : public Material
{
public:
	LineMaterial();

	void SetWorldPosition(ID3D11DeviceContext*, DirectX::XMFLOAT3&);
	void SetWorldRotation(ID3D11DeviceContext*, DirectX::XMMATRIX&);
	void SetLineWidthAndCameraPosition(ID3D11DeviceContext*, float, DirectX::XMFLOAT3&);
};

class SimpleMaterial : public Material
{
public:
	SimpleMaterial();
	void SetWorldPosition(ID3D11DeviceContext*, DirectX::XMMATRIX&);
};