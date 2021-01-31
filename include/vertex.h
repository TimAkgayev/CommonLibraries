#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#define NUM_MESH_VERTEX_ELEMENTS 4
#define NUM_LINE_VERTEX_ELEMENTS 2


struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 norm;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 uv;
};

struct LineVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
};