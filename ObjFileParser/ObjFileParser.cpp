#include <ObjFileParser.h>


class index_comparator
{
public:
	bool operator()(const tinyobj::index_t& lhv, const tinyobj::index_t& rhv) const
	{
		return std::tie(lhv.vertex_index, lhv.normal_index, lhv.texcoord_index) < std::tie(rhv.vertex_index, rhv.normal_index, rhv.texcoord_index);
	}
};

GenericObject ObjParser::ExtractByName(ObjFile & objFile, std::string objName)
{
	GenericObject o;

	std::vector<tinyobj::shape_t>& shapes = objFile.GetShapes();
	std::vector<tinyobj::material_t>& materials = objFile.GetMaterials();
	tinyobj::attrib_t& attributes = objFile.GetAttributes();

	// Loop over shapes and extract each object
	for (unsigned int s = 0; s < shapes.size(); s++)
	{
		if (shapes[s].name.compare(objName) != 0)
		{
			std::map<tinyobj::index_t, int, index_comparator> uniqueVertexMap;

			//go through each index and find unique entries
			for (tinyobj::index_t i : shapes[s].mesh.indices)
				uniqueVertexMap.insert(std::pair<tinyobj::index_t, int>(i, (int)uniqueVertexMap.size()));

			Vertex* vlist_temp = new Vertex[uniqueVertexMap.size()];

			for (auto& keyval : uniqueVertexMap)
			{
				tinyobj::real_t vx = attributes.vertices[3 * keyval.first.vertex_index + 0];
				tinyobj::real_t vy = attributes.vertices[3 * keyval.first.vertex_index + 1];
				tinyobj::real_t vz = attributes.vertices[3 * keyval.first.vertex_index + 2];

				tinyobj::real_t nx = attributes.normals[3 * keyval.first.normal_index + 0];
				tinyobj::real_t ny = attributes.normals[3 * keyval.first.normal_index + 1];
				tinyobj::real_t nz = attributes.normals[3 * keyval.first.normal_index + 2];

				tinyobj::real_t tx = attributes.texcoords[2 * keyval.first.texcoord_index + 0];
				tinyobj::real_t ty = attributes.texcoords[2 * keyval.first.texcoord_index + 1];

				// Optional: vertex colors
				tinyobj::real_t red = attributes.colors[3 * keyval.first.vertex_index + 0];
				tinyobj::real_t green = attributes.colors[3 * keyval.first.vertex_index + 1];
				tinyobj::real_t blue = attributes.colors[3 * keyval.first.vertex_index + 2];

				//
				// per-face material
				//shapes[s].mesh.material_ids[f];

				Vertex vertex;

				vertex.pos.x = vx;
				vertex.pos.y = vy;
				vertex.pos.z = vz;

				vertex.norm.x = nx;
				vertex.norm.y = ny;
				vertex.norm.z = nz;

				vertex.uv.x = tx;
				vertex.uv.y = ty;

				vertex.color.x = red;
				vertex.color.y = green;
				vertex.color.z = blue;

				vlist_temp[keyval.second] = vertex;
			}

			//copy over the vertex list
			o.vertex_list = std::vector<Vertex>(vlist_temp, vlist_temp + uniqueVertexMap.size());

			//now re-index the old index list
			for (tinyobj::index_t i : shapes[s].mesh.indices)
				o.index_list.push_back(uniqueVertexMap[i]);

			//save the texture path
			if (materials.size())
				o.texture_path = materials[shapes[s].mesh.material_ids[0]].diffuse_texname;
			else
				o.texture_path = "standin_texture.jpg";


			delete[] vlist_temp;

			return o;
		}

	}

	return o;

}

GenericObject ObjParser::ExtractIfSingle(ObjFile & objFile)
{
	GenericObject o;

	std::vector<tinyobj::shape_t>& shapes = objFile.GetShapes();
	std::vector<tinyobj::material_t>& materials = objFile.GetMaterials();
	tinyobj::attrib_t& attributes = objFile.GetAttributes();

	// extract only if there is exactly one object in the file
	if (shapes.size() == 1)
	{

		std::map<tinyobj::index_t, int, index_comparator> uniqueVertexMap;

		//go through each index and find unique entries
		for (tinyobj::index_t i : shapes[0].mesh.indices)
			uniqueVertexMap.insert(std::pair<tinyobj::index_t, int>(i, (int)uniqueVertexMap.size()));

		Vertex* vlist_temp = new Vertex[uniqueVertexMap.size()];

		for (auto& keyval : uniqueVertexMap)
		{
			tinyobj::real_t vx = attributes.vertices[3 * keyval.first.vertex_index + 0];
			tinyobj::real_t vy = attributes.vertices[3 * keyval.first.vertex_index + 1];
			tinyobj::real_t vz = attributes.vertices[3 * keyval.first.vertex_index + 2];

			tinyobj::real_t nx = attributes.normals[3 * keyval.first.normal_index + 0];
			tinyobj::real_t ny = attributes.normals[3 * keyval.first.normal_index + 1];
			tinyobj::real_t nz = attributes.normals[3 * keyval.first.normal_index + 2];

			tinyobj::real_t tx = attributes.texcoords[2 * keyval.first.texcoord_index + 0];
			tinyobj::real_t ty = attributes.texcoords[2 * keyval.first.texcoord_index + 1];

			// Optional: vertex colors
			tinyobj::real_t red = attributes.colors[3 * keyval.first.vertex_index + 0];
			tinyobj::real_t green = attributes.colors[3 * keyval.first.vertex_index + 1];
			tinyobj::real_t blue = attributes.colors[3 * keyval.first.vertex_index + 2];

			//
			// per-face material
			//shapes[s].mesh.material_ids[f];

			Vertex vertex;

			vertex.pos.x = vx;
			vertex.pos.y = vy;
			vertex.pos.z = vz;

			vertex.norm.x = nx;
			vertex.norm.y = ny;
			vertex.norm.z = nz;

			vertex.uv.x = tx;
			vertex.uv.y = ty;

			vertex.color.x = red;
			vertex.color.y = green;
			vertex.color.z = blue;

			vlist_temp[keyval.second] = vertex;
		}

		//copy over the vertex list
		o.vertex_list = std::vector<Vertex>(vlist_temp, vlist_temp + uniqueVertexMap.size());

		//now re-index the old index list
		for (tinyobj::index_t i : shapes[0].mesh.indices)
			o.index_list.push_back(uniqueVertexMap[i]);

		//save the texture path
		if (materials.size())
			o.texture_path = materials[shapes[0].mesh.material_ids[0]].diffuse_texname;
		else
			o.texture_path = "standin_texture.jpg";

		//remove the old pointer
		delete[] vlist_temp;

		return o;


	}

	return o;
}

std::vector<GenericObject> ObjParser::ExtractByTag(ObjFile & objFile, std::string tag)
{
	std::vector<GenericObject> o_list;

	std::vector<tinyobj::shape_t>& shapes = objFile.GetShapes();
	std::vector<tinyobj::material_t>& materials = objFile.GetMaterials();
	tinyobj::attrib_t& attributes = objFile.GetAttributes();

	// Loop over shapes and extract each object
	for (unsigned int s = 0; s < shapes.size(); s++)
	{
		if (shapes[s].name.find(tag) != std::string::npos)
		{
			std::map<tinyobj::index_t, int, index_comparator> uniqueVertexMap;

			//go through each index and find unique entries
			for (tinyobj::index_t i : shapes[s].mesh.indices)
				uniqueVertexMap.insert(std::pair<tinyobj::index_t, int>(i, (int)uniqueVertexMap.size()));

			Vertex* vlist_temp = new Vertex[uniqueVertexMap.size()];

			for (auto& keyval : uniqueVertexMap)
			{
				tinyobj::real_t vx = attributes.vertices[3 * keyval.first.vertex_index + 0];
				tinyobj::real_t vy = attributes.vertices[3 * keyval.first.vertex_index + 1];
				tinyobj::real_t vz = attributes.vertices[3 * keyval.first.vertex_index + 2];

				tinyobj::real_t nx = attributes.normals[3 * keyval.first.normal_index + 0];
				tinyobj::real_t ny = attributes.normals[3 * keyval.first.normal_index + 1];
				tinyobj::real_t nz = attributes.normals[3 * keyval.first.normal_index + 2];

				tinyobj::real_t tx = attributes.texcoords[2 * keyval.first.texcoord_index + 0];
				tinyobj::real_t ty = attributes.texcoords[2 * keyval.first.texcoord_index + 1];

				// Optional: vertex colors
				tinyobj::real_t red = attributes.colors[3 * keyval.first.vertex_index + 0];
				tinyobj::real_t green = attributes.colors[3 * keyval.first.vertex_index + 1];
				tinyobj::real_t blue = attributes.colors[3 * keyval.first.vertex_index + 2];

				//
				// per-face material
				//shapes[s].mesh.material_ids[f];

				Vertex vertex;

				vertex.pos.x = vx;
				vertex.pos.y = vy;
				vertex.pos.z = vz;

				vertex.norm.x = nx;
				vertex.norm.y = ny;
				vertex.norm.z = nz;

				vertex.uv.x = tx;
				vertex.uv.y = ty;

				vertex.color.x = red;
				vertex.color.y = green;
				vertex.color.z = blue;

				vlist_temp[keyval.second] = vertex;
			}

			GenericObject o;
			o.vertex_list = std::vector<Vertex>(vlist_temp, vlist_temp + uniqueVertexMap.size());

			//now re-index the old index list
			for (tinyobj::index_t i : shapes[s].mesh.indices)
				o.index_list.push_back(uniqueVertexMap[i]);

			//save the texture path
			if (materials.size())
				o.texture_path = materials[shapes[s].mesh.material_ids[0]].diffuse_texname;
			else
				o.texture_path = "standin_texture.jpg";

			o_list.push_back(o);

			//delete the temporary vertex list;
			delete[] vlist_temp;
		}

	}

	return o_list;
}



