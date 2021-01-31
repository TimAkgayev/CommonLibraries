#pragma once
#include <d3d11.h>
#include <vector>

struct VertexShader
{

	ID3D11VertexShader* GenericShader;
	std::vector<ID3D11Buffer*> ShaderConstantBuffers;

};

struct PixelShader
{

	ID3D11PixelShader* GenericShader;
	std::vector<ID3D11Buffer*> ShaderConstantBuffers;

};
struct GeometryShader
{

	ID3D11GeometryShader* GenericShader;
	std::vector<ID3D11Buffer*> ShaderConstantBuffers;

};
struct HullShader
{

	ID3D11HullShader* GenericShader;
	std::vector<ID3D11Buffer*> ShaderConstantBuffers;

};
struct DomainShader
{

	ID3D11DomainShader* GenericShader;
	std::vector<ID3D11Buffer*> ShaderConstantBuffers;

};



void CompileAndCreateShader(ID3D11Device* device, std::wstring filename, std::string entrypoint, ID3D11VertexShader** outShader, ID3DBlob** outBlob = NULL);
void CompileAndCreateShader(ID3D11Device* device, std::wstring filename, std::string entrypoint, ID3D11PixelShader** outShader, ID3DBlob** outBlob = NULL);
void CompileAndCreateShader(ID3D11Device* device, std::wstring filename, std::string entrypoint, ID3D11GeometryShader** outShader, ID3DBlob** outBlob = NULL);
