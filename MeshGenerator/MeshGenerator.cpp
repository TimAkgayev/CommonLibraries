#include <MeshGenerator.h>
using namespace DirectX;

namespace MeshGenerator
{
	void Box(const DirectX::BoundingBox & aabox, std::vector<Vertex>& outVB, std::vector<UINT>& outIB)
	{
		outVB.clear();
		outIB.clear();

		Vertex v;
		v.pos = XMFLOAT3(aabox.Center.x + aabox.Extents.x, aabox.Center.y -aabox.Extents.y, aabox.Center.z + aabox.Extents.z);
		v.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		outVB.push_back(v);

		v.pos = XMFLOAT3(aabox.Center.x - aabox.Extents.x, aabox.Center.y - aabox.Extents.y, aabox.Center.z + aabox.Extents.z);
		v.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		outVB.push_back(v);

		v.pos = XMFLOAT3(aabox.Center.x - aabox.Extents.x, aabox.Center.y + aabox.Extents.y, aabox.Center.z + aabox.Extents.z);
		v.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		outVB.push_back(v);

		v.pos = XMFLOAT3(aabox.Center.x + aabox.Extents.x, aabox.Center.y + aabox.Extents.y, aabox.Center.z + aabox.Extents.z);
		v.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		outVB.push_back(v);

		v.pos = XMFLOAT3(aabox.Center.x - aabox.Extents.x, aabox.Center.y + aabox.Extents.y, aabox.Center.z - aabox.Extents.z);
		v.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		outVB.push_back(v);

		v.pos = XMFLOAT3(aabox.Center.x - aabox.Extents.x, aabox.Center.y - aabox.Extents.y, aabox.Center.z - aabox.Extents.z);
		v.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		outVB.push_back(v);

		v.pos = XMFLOAT3(aabox.Center.x + aabox.Extents.x, aabox.Center.y - aabox.Extents.y, aabox.Center.z - aabox.Extents.z);
		v.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		outVB.push_back(v);

		v.pos = XMFLOAT3(aabox.Center.x + aabox.Extents.x, aabox.Center.y + aabox.Extents.y, aabox.Center.z - aabox.Extents.z);
		v.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		outVB.push_back(v);

		//assuming right handed system and clockwise winding order

		//front face
		outIB.push_back(0);
		outIB.push_back(1);
		outIB.push_back(3);
		outIB.push_back(3);
		outIB.push_back(1);
		outIB.push_back(2);

		//left face
		outIB.push_back(5);
		outIB.push_back(2);
		outIB.push_back(1);
		outIB.push_back(5);
		outIB.push_back(4);
		outIB.push_back(2);

		//rear face
		outIB.push_back(4);
		outIB.push_back(5);
		outIB.push_back(6);
		outIB.push_back(7);
		outIB.push_back(4);
		outIB.push_back(6);

		//right face
		outIB.push_back(3);
		outIB.push_back(7);
		outIB.push_back(6);
		outIB.push_back(6);
		outIB.push_back(0);
		outIB.push_back(3);

		//top face
		outIB.push_back(2);
		outIB.push_back(7);
		outIB.push_back(3);
		outIB.push_back(2);
		outIB.push_back(4);
		outIB.push_back(7);
		
		//bottom face
		outIB.push_back(1);
		outIB.push_back(0);
		outIB.push_back(6);
		outIB.push_back(1);
		outIB.push_back(6);
		outIB.push_back(5);

	}

	void Box(const DirectX::BoundingBox & aabox, std::vector<LineVertex>& outVB, std::vector<UINT>& outIB)
	{
		outVB.clear();
		outIB.clear();

		LineVertex v;
		v.pos = XMFLOAT3(aabox.Center.x + aabox.Extents.x, aabox.Center.y - aabox.Extents.y, aabox.Center.z + aabox.Extents.z);
		v.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		outVB.push_back(v);

		v.pos = XMFLOAT3(aabox.Center.x - aabox.Extents.x, aabox.Center.y - aabox.Extents.y, aabox.Center.z + aabox.Extents.z);
		v.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		outVB.push_back(v);

		v.pos = XMFLOAT3(aabox.Center.x - aabox.Extents.x, aabox.Center.y + aabox.Extents.y, aabox.Center.z + aabox.Extents.z);
		v.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		outVB.push_back(v);

		v.pos = XMFLOAT3(aabox.Center.x + aabox.Extents.x, aabox.Center.y + aabox.Extents.y, aabox.Center.z + aabox.Extents.z);
		v.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		outVB.push_back(v);

		v.pos = XMFLOAT3(aabox.Center.x - aabox.Extents.x, aabox.Center.y + aabox.Extents.y, aabox.Center.z - aabox.Extents.z);
		v.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		outVB.push_back(v);

		v.pos = XMFLOAT3(aabox.Center.x - aabox.Extents.x, aabox.Center.y - aabox.Extents.y, aabox.Center.z - aabox.Extents.z);
		v.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		outVB.push_back(v);

		v.pos = XMFLOAT3(aabox.Center.x + aabox.Extents.x, aabox.Center.y - aabox.Extents.y, aabox.Center.z - aabox.Extents.z);
		v.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		outVB.push_back(v);

		v.pos = XMFLOAT3(aabox.Center.x + aabox.Extents.x, aabox.Center.y + aabox.Extents.y, aabox.Center.z - aabox.Extents.z);
		v.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		outVB.push_back(v);

		//index buffer for a line strip

		//front face
		outIB.push_back(0);
		outIB.push_back(1);
		outIB.push_back(1);
		outIB.push_back(2);
		outIB.push_back(2);
		outIB.push_back(3);
		outIB.push_back(3);
		outIB.push_back(0);

		//left face
		outIB.push_back(1);
		outIB.push_back(5);
		outIB.push_back(5);
		outIB.push_back(4);
		outIB.push_back(4);
		outIB.push_back(2);
		outIB.push_back(2);
		outIB.push_back(1);

		//right face
		outIB.push_back(0);
		outIB.push_back(3);
		outIB.push_back(3);
		outIB.push_back(7);
		outIB.push_back(7);
		outIB.push_back(6);
		outIB.push_back(6);
		outIB.push_back(0);

		//top face
		outIB.push_back(2);
		outIB.push_back(4);
		outIB.push_back(4);
		outIB.push_back(7);
		outIB.push_back(7);
		outIB.push_back(3);
		outIB.push_back(3);
		outIB.push_back(2);

		//bottom face
		outIB.push_back(1);
		outIB.push_back(0);
		outIB.push_back(0);
		outIB.push_back(6);
		outIB.push_back(6);
		outIB.push_back(5);
		outIB.push_back(5);
		outIB.push_back(1);

		//back face
		outIB.push_back(4);
		outIB.push_back(5);
		outIB.push_back(5);
		outIB.push_back(6);
		outIB.push_back(6);
		outIB.push_back(7);
		outIB.push_back(7);
		outIB.push_back(4);




	}


	void Circle(float radius, UINT numPoints, std::vector<LineVertex>& outVB, std::vector<UINT>& outIB)
	{
		outVB.clear();
		outIB.clear();

		float inc = XM_2PI / numPoints;
		float radAng = 0.0f;

		for (UINT i = 0; i < numPoints; i++)
		{
			LineVertex v;
			v.pos = XMFLOAT3(radius*cos(radAng), radius*sin(radAng), 0.0f);
			v.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			outVB.push_back(v);

			radAng += inc;
		}

		for (UINT i = 0; i < outVB.size(); i++)
		{
			if (i == outVB.size() - 1)
			{
				outIB.push_back(i);
				outIB.push_back(0);
			}
			else
			{
				outIB.push_back(i);
				outIB.push_back(i + 1);
			}
		}
		

	}

}