#pragma once
#include <vector>
#include <vertex.h>
#include <DirectXCollision.h>

namespace MeshGenerator
{
	void Box(const DirectX::BoundingBox& aabox, std::vector<Vertex>& outVB, std::vector<UINT>& outIB);
	void Box(const DirectX::BoundingBox& aabox, std::vector<LineVertex>& outVB, std::vector<UINT>& outIB);

	void Circle(float radius, UINT numPoints, std::vector<LineVertex>& outVB, std::vector<UINT>& outIB);
}