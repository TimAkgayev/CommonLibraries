#pragma once
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vertex.h>
#include <DXAModel.h>

#define DEFAULT_LINE_COLOR DirectX::XMFLOAT4(1.0f,0.0f,0.0f,1.0f)

class Line : public DXAModel
{
public:
	Line(ID3D11Device* device, DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT4 c1 = DEFAULT_LINE_COLOR, DirectX::XMFLOAT4 c2 = DEFAULT_LINE_COLOR);
	Line(ID3D11Device* device, std::vector<LineVertex>& vertex_buffer, std::vector<UINT>& index_buffer);
	
	virtual void Draw(ID3D11DeviceContext* context) override;

};

