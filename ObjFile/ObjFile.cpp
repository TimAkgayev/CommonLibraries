#include "ObjFile.h"
#include "DirectXApplication.h"
#include <Utility.h>



ObjFile::ObjFile()
{
}

ObjFile::~ObjFile()
{
}

void ObjFile::LoadFile(ID3D11Device* device, std::string filename)
{
	if (_openfile(filename) == FAIL)
		return;

}

std::vector<tinyobj::material_t>& ObjFile::GetMaterials() 
{
	return m_materials;
	// TODO: insert return statement here
}

std::vector<tinyobj::shape_t>& ObjFile::GetShapes() 
{
	return m_shapes;
	// TODO: insert return statement here
}

tinyobj::attrib_t & ObjFile::GetAttributes() 
{
	return m_attributes;
	// TODO: insert return statement here
}

bool ObjFile::_openfile(std::string filename)
{
	std::string warn;
	std::string err;
	std::string base_dir = Utility::GetBaseDir(filename);
	tinyobj::LoadObj(&m_attributes, &m_shapes, &m_materials, &warn, &err, filename.c_str(), base_dir.c_str());
	
	if (!err.empty())
		return FAIL;
	else return OK;
}
