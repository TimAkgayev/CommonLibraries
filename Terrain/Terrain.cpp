#include <Terrain.h>
#include <Utility.h>
#include <WICTextureLoader.h>

char FindLongestSide(_bounding_box& box)
{
	XMFLOAT3 min, max;
	min = XMFLOAT3(box.center.x - box.x_extent, box.center.y - box.y_extent, box.center.z - box.z_extent);
	max = XMFLOAT3(box.center.x + box.x_extent, box.center.y + box.y_extent, box.center.z + box.z_extent);

	float x_length = max.x - min.x;
	float y_length = max.y - min.y;
	float z_length = max.z - min.z;


	char side;

	if (x_length > y_length)
	{
		if (x_length > z_length) // x is the largest
			side = 'x';
		else //z is the largest
			side = 'z';

	}
	else
	{
		if (y_length > z_length) //y is the largest
			side = 'y';
		else //z is the larges
			side = 'z';

	}

	return side;

}


int compare_func_x(const void* a, const void* b)
{
	_bounding_box* mma = (_bounding_box*)a;
	_bounding_box* mmb = (_bounding_box*)b;

	if (mma->center.x < mmb->center.x)
		return -1;
	else if (mma->center.x > mmb->center.x)
		return 1;
	else return 0;
}
int compare_func_y(const void* a, const void* b)
{
	_bounding_box* mma = (_bounding_box*)a;
	_bounding_box* mmb = (_bounding_box*)b;

	if (mma->center.y < mmb->center.y)
		return -1;
	else if (mma->center.y > mmb->center.y)
		return 1;
	else return 0;
}
int compare_func_z(const void* a, const void* b)
{
	_bounding_box* mma = (_bounding_box*)a;
	_bounding_box* mmb = (_bounding_box*)b;

	if (mma->center.z < mmb->center.z)
		return -1;
	else if (mma->center.z > mmb->center.z)
		return 1;
	else return 0;
}


void OrganizeListByAxis(char axis, std::vector<_bounding_box_tri>& boxList)
{


	switch (axis)
	{
	case 'x': {
		std::qsort(&boxList[0], boxList.size(), sizeof(_bounding_box_tri), compare_func_x);
	}break;
	case 'y': {
		std::qsort(&boxList[0], boxList.size(), sizeof(_bounding_box_tri), compare_func_y);
	}break;
	case 'z': {
		std::qsort(&boxList[0], boxList.size(), sizeof(_bounding_box_tri), compare_func_z);
	}break;

	default: break;
	}

}


void FindBoundingBox(Vertex* vertexList, UINT numVerts, _bounding_box_tri& outBox)
{
	//find the smallest and largest values
	std::vector<float> vertexXList;

	float smallest_x = 1000000.0f;
	float largest_x = -1000000.0f;

	float smallest_y = 1000000.0f;
	float largest_y = -1000000.0f;

	float smallest_z = 1000000.0f;
	float largest_z = -1000000.0f;

	for (UINT i = 0; i < numVerts; i++)
	{

		if (tri_list[i].vertex[0].x < smallest_x)
			smallest_x = tri_list[i].vertex[0].x;
		if (tri_list[i].vertex[0].x > largest_x)
			largest_x = tri_list[i].vertex[0].x;


		if (tri_list[i].vertex[0].y < smallest_y)
			smallest_y = tri_list[i].vertex[0].y;
		if (tri_list[i].vertex[0].y > largest_y)
			largest_y = tri_list[i].vertex[0].y;

		if (tri_list[i].vertex[0].z < smallest_z)
			smallest_z = tri_list[i].vertex[0].z;
		if (tri_list[i].vertex[0].z > largest_z)
			largest_z = tri_list[i].vertex[0].z;

	}

	outBox.center = XMFLOAT3((largest_x + smallest_x) / 2, (largest_y + smallest_y) / 2, (largest_z + smallest_z) / 2);
	outBox.x_extent = (largest_x - smallest_x) / 2;
	outBox.y_extent = (largest_y - smallest_y) / 2;
	outBox.z_extent = (largest_z - smallest_z) / 2;
}


_bounding_box FindBoundingBox(const std::vector<_bounding_box_tri>& boxList)
{

	std::vector<XMFLOAT3> minList;
	std::vector<XMFLOAT3> maxList;

	for (UINT i = 0; i < boxList.size(); i++)
	{
		XMVECTOR boxCenter = XMLoadFloat3(&boxList[i].center);


		XMFLOAT3 min, max;
		min = XMFLOAT3(boxList[i].center.x - boxList[i].x_extent, boxList[i].center.y - boxList[i].y_extent, boxList[i].center.z - boxList[i].z_extent);
		max = XMFLOAT3(boxList[i].center.x + boxList[i].x_extent, boxList[i].center.y + boxList[i].y_extent, boxList[i].center.z + boxList[i].z_extent);


		minList.push_back(min);
		maxList.push_back(max);

	}


	float smallest_x = 1000000.0f;
	float largest_x = -1000000.0f;

	float smallest_y = 1000000.0f;
	float largest_y = -1000000.0f;

	float smallest_z = 1000000.0f;
	float largest_z = -1000000.0f;

	for (UINT i = 0; i < boxList.size(); i++)
	{

		if (minList[i].x < smallest_x)
			smallest_x = minList[i].x;
		if (maxList[i].x > largest_x)
			largest_x = maxList[i].x;


		if (minList[i].y < smallest_y)
			smallest_y = minList[i].y;
		if (maxList[i].y > largest_y)
			largest_y = maxList[i].y;

		if (minList[i].z < smallest_z)
			smallest_z = minList[i].z;
		if (maxList[i].z > largest_z)
			largest_z = maxList[i].z;

	}

	_bounding_box outBox;
	outBox.center = XMFLOAT3((largest_x + smallest_x) / 2, (largest_y + smallest_y) / 2, (largest_z + smallest_z) / 2);
	outBox.x_extent = (largest_x - smallest_x) / 2;
	outBox.y_extent = (largest_y - smallest_y) / 2;
	outBox.z_extent = (largest_z - smallest_z) / 2;

	return outBox;
}




std::vector<_heap_node> GenerateBVHTree(const std::vector<_bounding_box_tri>& terrainTriBBList, const UINT unitsPerLeaf = 100)
{

	std::vector<_heap_node> BVHTree;

	//if no triangles return an empty list
	if (!terrainTriBBList.size())
		return BVHTree;

	_heap_node root;

	//assign all triangles to root
	root.triangleList = terrainTriBBList;
	root.bounding_box = FindBoundingBox(terrainTriBBList);
	BVHTree.push_back(root);

	std::vector<size_t> stack;

	//if root has more triangles associated with it then the maximum allowed, split it
	if (root.triangleList.size() > unitsPerLeaf)
		stack.push_back(0);

	//go through each node that exceedes the max leaves and split
	for (unsigned int i = 0; i < stack.size(); i++)
	{
		size_t node_index = stack[i];

		//sort the triangles in this node before splitting
		std::vector<_bounding_box_tri>& sortedTriangles = BVHTree[node_index].triangleList;
		char longestAxis = FindLongestSide(BVHTree[node_index].bounding_box);
		OrganizeListByAxis(longestAxis, sortedTriangles);

		//split off the left side
		size_t split_start = 0;
		size_t split_end = int(sortedTriangles.size() / 2.0f + 0.5f);

		std::vector<_bounding_box_tri>::const_iterator startIter = sortedTriangles.begin() + split_start;
		std::vector<_bounding_box_tri>::const_iterator endIter = sortedTriangles.begin() + split_end;
		std::vector<_bounding_box_tri> leftsideTriangleList(startIter, endIter);

		//avoid empty nodes
		if (leftsideTriangleList.size())
		{
			_heap_node leftChild;
			leftChild.bounding_box = FindBoundingBox(leftsideTriangleList);
			leftChild.triangleList = leftsideTriangleList;

	

			size_t leftChild_index = BVHTree.size();
			BVHTree[node_index].childIndices[0] = leftChild_index;

			//add child to tree
			BVHTree.push_back(leftChild);

			//if child needs splitting, do so
			if (leftsideTriangleList.size() > unitsPerLeaf)
				stack.push_back(leftChild_index);
		}

		//split off the right side
		split_start = split_end;
		split_end = BVHTree[node_index].triangleList.size();

		std::vector<_bounding_box_tri>& sortedTriangles2 = BVHTree[node_index].triangleList;


		//create a sublist of triangle bounding boxes that are contained in this split
		startIter = sortedTriangles2.begin() + split_start;
		endIter = sortedTriangles2.begin() + split_end;
		std::vector<_bounding_box_tri> rightsideTriangleList(startIter, endIter);

		//avoid empty nodes
		if (rightsideTriangleList.size())
		{
			_heap_node rightChild;
			rightChild.bounding_box = FindBoundingBox(rightsideTriangleList);
			rightChild.triangleList = rightsideTriangleList;

			size_t rightChild_index = BVHTree.size();
			BVHTree[node_index].childIndices[1] = rightChild_index;

			//add child to tree
			BVHTree.push_back(rightChild);

			//if child needs splitting, do so
			if (rightsideTriangleList.size() > unitsPerLeaf)
				stack.push_back(rightChild_index);
		}
	}

	return BVHTree;

}

class index_comparator
{
public:
	bool operator()(const tinyobj::index_t& lhv, const tinyobj::index_t& rhv) const
	{
		return std::tie(lhv.vertex_index, lhv.normal_index, lhv.texcoord_index) < std::tie(rhv.vertex_index, rhv.normal_index, rhv.texcoord_index);
	}
};


Terrain::Terrain()
{
}

Terrain::~Terrain()
{
}

void Terrain::LoadFromMemory(ID3D11Device* device, tinyobj::shape_t & shape, tinyobj::attrib_t & attrib, std::vector<tinyobj::material_t>& materials, std::string filename)
{
	std::map<tinyobj::index_t, int, index_comparator> uniqueVertexMap;

	//go through each index and find unique entries
	for (tinyobj::index_t i : shape.mesh.indices)
		uniqueVertexMap.insert(std::pair<tinyobj::index_t, int>(i, (int)uniqueVertexMap.size()));


	//allocate space for the vertices
	mVertices = new Vertex[uniqueVertexMap.size()];
	mNumVertices = uniqueVertexMap.size();

	for (auto& keyval : uniqueVertexMap)
	{
		tinyobj::real_t vx = attrib.vertices[3 * keyval.first.vertex_index + 0];
		tinyobj::real_t vy = attrib.vertices[3 * keyval.first.vertex_index + 1];
		tinyobj::real_t vz = attrib.vertices[3 * keyval.first.vertex_index + 2];

		tinyobj::real_t nx = attrib.normals[3 * keyval.first.normal_index + 0];
		tinyobj::real_t ny = attrib.normals[3 * keyval.first.normal_index + 1];
		tinyobj::real_t nz = attrib.normals[3 * keyval.first.normal_index + 2];

		tinyobj::real_t tx = attrib.texcoords[2 * keyval.first.texcoord_index + 0];
		tinyobj::real_t ty = attrib.texcoords[2 * keyval.first.texcoord_index + 1];

		// Optional: vertex colors
		// tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
		// tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
		// tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];

		//
		// per-face material
		//shapes[s].mesh.material_ids[f];

		Vertex vert;

		vert.pos.x = vx;
		vert.pos.y = vy;
		vert.pos.z = vz;

		vert.norm.x = nx;
		vert.norm.y = ny;
		vert.norm.z = nz;

		vert.uv.x = tx;
		vert.uv.y = ty;

		mVertices[keyval.second] = vert;
	}

	//now re-index the old index list
	for (tinyobj::index_t i : shape.mesh.indices)
		mIndexList.push_back(uniqueVertexMap[i]);


	//find boundaries
	float smallest_x = 100000000.0f;
	float smallest_y = 100000000.0f;
	float smallest_z = 100000000.0f;
	float largest_x = -100000000.0f;
	float largest_y = -100000000.0f;
	float largest_z = -100000000.0f;

	for (int i = 0; i < mNumVertices; i++)
	{
		Vertex v = mVertices[i];
		if (v.pos.x < smallest_x)
			smallest_x = v.pos.x;
		if (v.pos.x > largest_x)
			largest_x = v.pos.x;
		if (v.pos.y < smallest_y)
			smallest_y = v.pos.y;
		if (v.pos.y > largest_y)
			largest_y = v.pos.y;
		if (v.pos.z < smallest_z)
			smallest_z = v.pos.z;
		if (v.pos.z > largest_z)
			largest_z = v.pos.z;
	}

	//load the texture
	std::string textureName = materials[shape.mesh.material_ids[0]].diffuse_texname;
	std::string base_dir = Utility::GetBaseDir(filename);
	std::string texturePath = base_dir + "/" + textureName;
	std::wstring texturePathW = Utility::StringToWString(texturePath);
	CreateWICTextureFromFile(device, texturePathW.c_str(), NULL, &mTextureSRV);


//	mBoundingRect.lowerLeft = XMFLOAT2(smallest_x, smallest_z);
	//mBoundingRect.upperRight = XMFLOAT2(largest_x, largest_z);

	/*
		//adjust the y so it's on zero
		if (smallest_y != 0)
		{

			float neg_y = -smallest_y;
			for (int i = 0; i < mNumVertices; i++)
				mVertices[i].pos.y += neg_y;

		}

		*/


		//create the index buffer
	D3D11_BUFFER_DESC bufferDesc;

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth =  sizeof(DWORD) * (UINT)mIndexList.size();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;


	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &mIndexList[0];
	device->CreateBuffer(&bufferDesc, &InitData, &mIB);

	mNumIndices = mIndexList.size();

	//create the vertex buffer
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex) * (UINT)mNumVertices; //total size of buffer in bytes
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = mVertices;

	device->CreateBuffer(&bufferDesc, &InitData, &mVB);



	//generate a BVH

	//find the longest side of bounding box
	float x_length = largest_x - smallest_x;
	float y_length = largest_y - smallest_y;
	float z_length = largest_z - smallest_z;

	//find which side is the longest
	char side;

	if (x_length > y_length)
	{
		if (x_length > z_length) // x is the largest
			side = 'x';
		else //z is the largest
			side = 'z';

	}
	else
	{
		if (y_length > z_length) //y is the largest
			side = 'y';
		else //z is the larges
			side = 'z';

	}

	std::vector<_bounding_box_tri> triBoundingBoxes;


	for (UINT triangle = 0; triangle < mIndexList.size(); triangle += 3)
	{
		Vertex v[3];

		v[0] = mVertices[mIndexList[triangle + 0]];
		v[1] = mVertices[mIndexList[triangle + 1]];
		v[2] = mVertices[mIndexList[triangle + 2]];


		_terrain_triangle t;
		t.vertices[0] = v[0];
		t.vertices[1] = v[1];
		t.vertices[2] = v[2];

		mTriangleList.push_back(t);


		_bounding_box_tri bb;
		FindBoundingBox(v, 3, bb);
		bb.tri_index = (UINT)mTriangleList.size() - 1;

		triBoundingBoxes.push_back(bb);

	}

	mBVHTree = GenerateBVHTree(triBoundingBoxes, 10);
}

void Terrain::_CreateTerrainVertexBuffersAndGenerateBVHTree(ID3D11Device* device, tinyobj::shape_t & shape, tinyobj::attrib_t & attrib, std::vector<tinyobj::material_t>& materials, std::string filename)
{

	std::map<tinyobj::index_t, int, index_comparator> uniqueVertexMap;

	//go through each index and find unique entries
	for (tinyobj::index_t i : shape.mesh.indices)
		uniqueVertexMap.insert(std::pair<tinyobj::index_t, int>(i, (int)uniqueVertexMap.size()));


	//allocate space for the vertices
	mVertices = new Vertex[uniqueVertexMap.size()];
	mNumVertices = uniqueVertexMap.size();

	for (auto& keyval : uniqueVertexMap)
	{
		tinyobj::real_t vx = attrib.vertices[3 * keyval.first.vertex_index + 0];
		tinyobj::real_t vy = attrib.vertices[3 * keyval.first.vertex_index + 1];
		tinyobj::real_t vz = attrib.vertices[3 * keyval.first.vertex_index + 2];

		tinyobj::real_t nx = attrib.normals[3 * keyval.first.normal_index + 0];
		tinyobj::real_t ny = attrib.normals[3 * keyval.first.normal_index + 1];
		tinyobj::real_t nz = attrib.normals[3 * keyval.first.normal_index + 2];

		tinyobj::real_t tx = attrib.texcoords[2 * keyval.first.texcoord_index + 0];
		tinyobj::real_t ty = attrib.texcoords[2 * keyval.first.texcoord_index + 1];

		// Optional: vertex colors
		// tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
		// tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
		// tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];

		//
		// per-face material
		//shapes[s].mesh.material_ids[f];

		Vertex vert;

		vert.pos.x = vx;
		vert.pos.y = vy;
		vert.pos.z = vz;

		vert.norm.x = nx;
		vert.norm.y = ny;
		vert.norm.z = nz;

		vert.uv.x = tx;
		vert.uv.y = ty;

		mVertices[keyval.second] = vert;
	}

	//now re-index the old index list
	for (tinyobj::index_t i : shape.mesh.indices)
		mIndexList.push_back(uniqueVertexMap[i]);


	//find boundaries
	float smallest_x = 100000000.0f;
	float smallest_y = 100000000.0f;
	float smallest_z = 100000000.0f;
	float largest_x = -100000000.0f;
	float largest_y = -100000000.0f;
	float largest_z = -100000000.0f;

	for (int i = 0; i < mNumVertices; i++)
	{
		Vertex v = mVertices[i];
		if (v.pos.x < smallest_x)
			smallest_x = v.pos.x;
		if (v.pos.x > largest_x)
			largest_x = v.pos.x;
		if (v.pos.y < smallest_y)
			smallest_y = v.pos.y;
		if (v.pos.y > largest_y)
			largest_y = v.pos.y;
		if (v.pos.z < smallest_z)
			smallest_z = v.pos.z;
		if (v.pos.z > largest_z)
			largest_z = v.pos.z;
	}

	//load the texture
	std::string textureName = materials[shape.mesh.material_ids[0]].diffuse_texname;
	std::string base_dir = Utility::GetBaseDir(filename);
	std::string texturePath = base_dir + "/" + textureName;
	std::wstring texturePathW = Utility::StringToWString(texturePath);
	CreateWICTextureFromFile(device, texturePathW.c_str(), NULL, &mTextureSRV);



	//adjust the y so it's on zero
	if (smallest_y != 0)
	{

		float neg_y = -smallest_y;
		for (int i = 0; i < mNumVertices; i++)
			mVertices[i].pos.y += neg_y;

	}


	//create the index buffer
	D3D11_BUFFER_DESC bufferDesc;

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(DWORD) * (UINT)mIndexList.size();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;


	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &mIndexList[0];
	device->CreateBuffer(&bufferDesc, &InitData, &mIB);

	mNumIndices = mIndexList.size();

	//create the vertex buffer
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex) * (UINT)mNumVertices; //total size of buffer in bytes
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = mVertices;

	device->CreateBuffer(&bufferDesc, &InitData, &mVB);



	//generate a BVH

	//find the longest side of bounding box
	float x_length = largest_x - smallest_x;
	float y_length = largest_y - smallest_y;
	float z_length = largest_z - smallest_z;

	//find which side is the longest
	char side;

	if (x_length > y_length)
	{
		if (x_length > z_length) // x is the largest
			side = 'x';
		else //z is the largest
			side = 'z';

	}
	else
	{
		if (y_length > z_length) //y is the largest
			side = 'y';
		else //z is the larges
			side = 'z';

	}



	std::vector<_bounding_box_tri> triBoundingBoxes;


	for (UINT triangle = 0; triangle < mIndexList.size(); triangle += 3)
	{
		Vertex v[3];

		v[0] = mVertices[mIndexList[triangle + 0]];
		v[1] = mVertices[mIndexList[triangle + 1]];
		v[2] = mVertices[mIndexList[triangle + 2]];


		_terrain_triangle t;
		t.vertices[0] = v[0];
		t.vertices[1] = v[1];
		t.vertices[2] = v[2];

		mTriangleList.push_back(t);


		_bounding_box_tri bb;
		FindBoundingBox(v, 3, bb);
		bb.tri_index = mTriangleList.size() - 1;

		triBoundingBoxes.push_back(bb);

	}

	mBVHTree = GenerateBVHTree(triBoundingBoxes, 10);
}
