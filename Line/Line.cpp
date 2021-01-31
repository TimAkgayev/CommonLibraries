#include <Line.h>
#include <vertex.h>
using namespace DirectX;

Line::Line(ID3D11Device* device, XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT4 c1, XMFLOAT4 c2)
{
	//create the vertex buffer
	LineVertex v[2] = { {p1, c1}, {p2,c2} };
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(LineVertex) * (UINT)2; //total size of buffer in bytes
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = &v[0];
	device->CreateBuffer(&bufferDesc, &InitData, &mVertexBuffer);

	//create the index buffer
	UINT i[2] = { 0, 1 };
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(DWORD) * (UINT)2;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = &i[0];
	mNumIndices = 2;
	device->CreateBuffer(&bufferDesc, &InitData, &mIndexBuffer);

	//define topology
	mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

}


Line::Line(ID3D11Device* device, std::vector<LineVertex>& vertex_buffer, std::vector<UINT>& index_buffer)
{
	//create the vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(LineVertex) * (UINT)vertex_buffer.size(); //total size of buffer in bytes
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = &vertex_buffer[0];
	device->CreateBuffer(&bufferDesc, &InitData, &mVertexBuffer);

	//create the index buffer
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(DWORD) * (UINT)index_buffer.size();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = &index_buffer[0];
	mNumIndices = (UINT)index_buffer.size();
	device->CreateBuffer(&bufferDesc, &InitData, &mIndexBuffer);

	//define topology
	mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

}


void Line::Draw(ID3D11DeviceContext * context)
{
	//Input Assembler
	UINT stride = sizeof(LineVertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(mTopology);

	context->DrawIndexed(mNumIndices, 0, 0);
}
