#pragma once
#include <vector>
#include <string>
#include <ObjFile.h>
#include <Vertex.h>

struct GenericObject
{
	std::vector<Vertex> vertex_list;
	std::vector<UINT> index_list;
	std::string texture_path;
};

class ObjParser
{
public:
	//extract an object only if the exact name matches
	static GenericObject ExtractByName(ObjFile& objFile, std::string objName);

	//extracts an object only if the file contains one single object
	static GenericObject ExtractIfSingle(ObjFile& objFile);

	//return all objects that have the tag word in their name
	static std::vector<GenericObject> ExtractByTag(ObjFile& objFile, std::string tag);
};