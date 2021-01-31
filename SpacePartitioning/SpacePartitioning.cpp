#include <SpacePartitioning.h>
using namespace DirectX;

namespace BVH
{


	XMFLOAT3 GetTriangleCenter(triangle& tri)
	{
		float x_avg = 0, y_avg = 0, z_avg = 0;
		for (int i = 0; i < 3; i++)
		{
			x_avg += tri.vertex[i].x;
			y_avg += tri.vertex[i].y;
			z_avg += tri.vertex[i].z;
		}

		return XMFLOAT3(x_avg / 3.0f, y_avg / 3.0f, z_avg / 3.0f);
	}

	char FindLongestAxis(DirectX::BoundingBox& aabb)
	{
		XMFLOAT3 min, max;
		min = XMFLOAT3(aabb.Center.x - aabb.Extents.x, aabb.Center.y - aabb.Extents.y, aabb.Center.z - aabb.Extents.z);
		max = XMFLOAT3(aabb.Center.x + aabb.Extents.x, aabb.Center.y + aabb.Extents.y, aabb.Center.z + aabb.Extents.z);

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
		triangle* triA = (triangle*)a;
		triangle* triB = (triangle*)b;

		XMFLOAT3 triACenter = GetTriangleCenter(*triA);
		XMFLOAT3 triBCenter = GetTriangleCenter(*triB);


		if (triACenter.x < triBCenter.x)
			return -1;
		else if (triACenter.x > triBCenter.x)
			return 1;
		else return 0;
	}

	int compare_func_y(const void* a, const void* b)
	{
		triangle* triA = (triangle*)a;
		triangle* triB = (triangle*)b;

		XMFLOAT3 triACenter = GetTriangleCenter(*triA);
		XMFLOAT3 triBCenter = GetTriangleCenter(*triB);


		if (triACenter.y < triBCenter.y)
			return -1;
		else if (triACenter.y > triBCenter.y)
			return 1;
		else return 0;
	}

	int compare_func_z(const void* a, const void* b)
	{
		triangle* triA = (triangle*)a;
		triangle* triB = (triangle*)b;

		XMFLOAT3 triACenter = GetTriangleCenter(*triA);
		XMFLOAT3 triBCenter = GetTriangleCenter(*triB);


		if (triACenter.z < triBCenter.z)
			return -1;
		else if (triACenter.z > triBCenter.z)
			return 1;
		else return 0;
	}

	void SortTrianglesByAxis(char axis, triangle* tri, UINT num_tri)
	{


		switch (axis)
		{
		case 'x': {
			std::qsort(tri, num_tri, sizeof(triangle), compare_func_x);
		}break;
		case 'y': {
			std::qsort(tri, num_tri, sizeof(triangle), compare_func_y);
		}break;
		case 'z': {
			std::qsort(tri, num_tri, sizeof(triangle), compare_func_z);
		}break;

		default: break;
		}

	}

	void GenerateTriangleListFromIndexedVertexBuffer(std::vector<Vertex>& vb, std::vector<UINT>& ib, triangle ** triangle_buffer, UINT & num_tri_out)
	{
		//if the index buffer is not in sets of 3, something is wrong
		if ((UINT)ib.size() % 3 != 0)
			return;

		num_tri_out = (UINT)ib.size() / 3;
		(*triangle_buffer) = new triangle[num_tri_out];
		for (UINT i = 0, t = 0; i < ib.size(); i += 3, t++)
		{
			(*triangle_buffer)[t].vertex[0] = vb[ib[i]].pos;
			(*triangle_buffer)[t].vertex[1] = vb[ib[i + 1]].pos;
			(*triangle_buffer)[t].vertex[2] = vb[ib[i + 2]].pos;
		}


		for (UINT i = 0; i < num_tri_out; i++)
		{
			XMFLOAT3 b[3];
			b[0] = (*triangle_buffer)[i].vertex[0];
			b[1] = (*triangle_buffer)[i].vertex[1];
			b[2] = (*triangle_buffer)[i].vertex[2];
			int n = 0;
		}

	}

	DirectX::BoundingBox FindBoundingBox(triangle * tri_list, UINT num_triangles)
	{
		//find the smallest and largest values
		std::vector<float> vertexXList;

		float smallest_x = 1000000.0f;
		float largest_x = -1000000.0f;

		float smallest_y = 1000000.0f;
		float largest_y = -1000000.0f;

		float smallest_z = 1000000.0f;
		float largest_z = -1000000.0f;

		for (UINT i = 0; i < num_triangles; i++)
		{

			for (UINT v = 0; v < 3; v++)
			{
				XMFLOAT3 b = tri_list[i].vertex[v];

				if (tri_list[i].vertex[v].x < smallest_x)
					smallest_x = tri_list[i].vertex[v].x;
				if (tri_list[i].vertex[v].x > largest_x)
					largest_x = tri_list[i].vertex[v].x;


				if (tri_list[i].vertex[v].y < smallest_y)
					smallest_y = tri_list[i].vertex[v].y;
				if (tri_list[i].vertex[v].y > largest_y)
					largest_y = tri_list[i].vertex[v].y;

				if (tri_list[i].vertex[v].z < smallest_z)
					smallest_z = tri_list[i].vertex[v].z;
				if (tri_list[i].vertex[v].z > largest_z)
					largest_z = tri_list[i].vertex[v].z;
			}

		}

		DirectX::BoundingBox outAABB;
		outAABB.Center = XMFLOAT3((largest_x + smallest_x) / 2, (largest_y + smallest_y) / 2, (largest_z + smallest_z) / 2);
		outAABB.Extents.x = (largest_x - smallest_x) / 2;
		outAABB.Extents.y = (largest_y - smallest_y) / 2;
		outAABB.Extents.z = (largest_z - smallest_z) / 2;

		return outAABB;
	}

	UINT PartitionVolume(triangle * tri, UINT num_tri)
	{
		//using a space partition 
		DirectX::BoundingBox bv = FindBoundingBox(tri, num_tri);

		char longestAxis = FindLongestAxis(bv);
		SortTrianglesByAxis(longestAxis, tri, num_tri);

		return UINT(num_tri / 2.0f + 0.5f);
	}

	node const * GenerateBoundingVolumeHeirarchy(triangle * tri_list, UINT num_triangles, UINT MAX_OBJECTS_PER_LEAF)
	{
		node* root = new node;
		root->triangle_list = tri_list;
		root->num_triangles = num_triangles;
		root->bounding_volume = FindBoundingBox(tri_list, num_triangles);
		root->left = nullptr;
		root->right = nullptr;
		root->parent = nullptr;

		std::vector<node*> stack;

		if (num_triangles > MAX_OBJECTS_PER_LEAF)
		{
			stack.push_back(root);

			for (UINT i = 0; i < stack.size(); i++)
			{
				UINT partitionPoint = PartitionVolume(stack[i]->triangle_list, stack[i]->num_triangles);
				node* left = new node;
				left->parent = stack[i];
				left->triangle_list = stack[i]->triangle_list;
				left->num_triangles = partitionPoint;
				left->left = nullptr;
				left->right = nullptr;
				left->bounding_volume = FindBoundingBox(left->triangle_list, left->num_triangles);
				stack[i]->left = left;


				node* right = new node;
				right->parent = stack[i];
				right->triangle_list = stack[i]->triangle_list + partitionPoint;
				right->num_triangles = stack[i]->num_triangles - partitionPoint;
				right->left = nullptr;
				right->right = nullptr;
				right->bounding_volume = FindBoundingBox(right->triangle_list, right->num_triangles);
				stack[i]->right = right;


				if (left->num_triangles > MAX_OBJECTS_PER_LEAF)
					stack.push_back(left);

				if (right->num_triangles > MAX_OBJECTS_PER_LEAF)
					stack.push_back(right);

			}

		}

		return root;
	}


}