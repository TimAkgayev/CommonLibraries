#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <Vertex.h>
#include <DirectXCollision.h>
namespace BVH
{

	struct triangle
	{
		DirectX::XMFLOAT3 vertex[3];
	};

	struct node
	{
		node* left;
		node* right;
		node* parent;

		triangle* triangle_list;
		UINT num_triangles;
		DirectX::BoundingBox bounding_volume;
	};



	//converts a vertex buffer into a readable triangle list, allocates a buffer
	void GenerateTriangleListFromIndexedVertexBuffer(std::vector<Vertex>& vb, std::vector<UINT>& ib, triangle** triangle_buffer, UINT& num_tri_out);

	//returns the bounding box around a group of triangles
	DirectX::BoundingBox FindBoundingBox(triangle* tri_list, UINT num_triangles);

	//finds a partition point in a list of triangles, might alter the ordering of the list
	UINT PartitionVolume(triangle* tri, UINT num_tri);

	node const* GenerateBoundingVolumeHeirarchy(triangle* tri_list, UINT num_triangles, UINT MAX_OBJECTS_PER_LEAF);

};
