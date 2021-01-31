#pragma once
#include <vector>
#include <string>
#include <tiny_obj_loader.h>
#include <d3d11.h>

#define FAIL 0
#define OK 1

class ObjFile
{
public:
	ObjFile();
	~ObjFile();

	void LoadFile(ID3D11Device* device, std::string filename);

	tinyobj::attrib_t& GetAttributes();
	std::vector<tinyobj::shape_t>& GetShapes();
	std::vector<tinyobj::material_t>& GetMaterials();

private:
	tinyobj::attrib_t m_attributes;
	std::vector<tinyobj::shape_t> m_shapes;
	std::vector<tinyobj::material_t> m_materials;

private:
	bool _openfile(std::string filename);
};

