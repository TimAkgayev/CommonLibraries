#pragma once
#include <DirectXApplication.h>
#include <tiny_obj_loader.h>

struct _bounding_box_tri
{
	XMFLOAT3 center;
	float    x_extent;
	float    y_extent;
	float    z_extent;

	size_t tri_index;
};

struct _bounding_box
{
	XMFLOAT3 center;
	float    x_extent;
	float    y_extent;
	float    z_extent;

};

struct _heap_node
{

	//the bounding box of this volume
	_bounding_box bounding_box;

	//the list of triangles this volume encompasses
	std::vector<_bounding_box_tri> triangleList;

	//left and right branches
	size_t childIndices[2];

	_heap_node()
	{
		childIndices[0] = -1;
		childIndices[1] = -1;
	}

};


struct _terrain_triangle
{
	Vertex vertices[3];
};

class Terrain 
{
public:
	Terrain();
	virtual ~Terrain();

	void LoadFromMemory(ID3D11Device* device, tinyobj::shape_t& shape, tinyobj::attrib_t& attrib, std::vector<tinyobj::material_t>& materials, std::string filename);

private:

	ID3D11ShaderResourceView* mTextureSRV;


	size_t mNumVertices;
	size_t mNumIndices;

	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	std::vector<_heap_node> mBVHTree;
	Vertex*	mVertices;

	std::vector<DWORD> mIndexList;
	std::vector<_terrain_triangle> mTriangleList;

	void _CreateTerrainVertexBuffersAndGenerateBVHTree(ID3D11Device* device, tinyobj::shape_t& shape, tinyobj::attrib_t& attrib, std::vector<tinyobj::material_t>& materials, std::string filename);
};