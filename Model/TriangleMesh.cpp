#include "TriangleMesh.h"
#include <WICTextureLoader.h>
#include "Utility.h"

#define DEFAULT_TOPOLOGY D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST


TriangleMesh::TriangleMesh(ID3D11Device* device, GenericObject LoadedObject)
{

	//create the index buffer
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(DWORD) * (UINT)LoadedObject.index_list.size();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &LoadedObject.index_list[0];
	mNumIndices = (UINT)LoadedObject.index_list.size();
	device->CreateBuffer(&bufferDesc, &InitData, &mIndexBuffer);

	//create the vertex buffer
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex) * (UINT)LoadedObject.vertex_list.size(); //total size of buffer in bytes
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = &LoadedObject.vertex_list[0];
	device->CreateBuffer(&bufferDesc, &InitData, &mVertexBuffer);

	//save the GenericObject for future use
	mObject = LoadedObject;

	//define topology
	mTopology = DEFAULT_TOPOLOGY;

	//load texture
	DirectX::CreateWICTextureFromFile(device, Utility::StringToWString(LoadedObject.texture_path).c_str(), NULL, &mPSResourceView);


	//create a sampler
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;
	HandleError(device->CreateSamplerState(&samplerDesc, &mPSSampler));


	//Generate a bounding volume heirarchy
	UINT num_terrain_triangles;
	BVH::GenerateTriangleListFromIndexedVertexBuffer(GetObject().vertex_list, GetObject().index_list, &mTriangleList, mNumTriangles);
	mTree = BVH::GenerateBoundingVolumeHeirarchy(mTriangleList, mNumTriangles, 200);
}



void TriangleMesh::Draw(ID3D11DeviceContext * context)
{
	//Input Assembler
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(mTopology);

	//Pixel Shader
	context->PSSetShaderResources(0, 1, &mPSResourceView);
	context->PSSetSamplers(0, 1, &mPSSampler);

	context->DrawIndexed(mNumIndices, 0, 0);
}

GenericObject & TriangleMesh::GetObject()
{
	// TODO: insert return statement here
	return mObject;
}

const BVH::node * TriangleMesh::GetTree() const
{
	return mTree;
}
