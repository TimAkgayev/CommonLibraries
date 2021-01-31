#include <GenericShader.h>
#include <Utility.h>

void CompileAndCreateShader(ID3D11Device* device, std::wstring filename, std::string entrypoint, ID3D11VertexShader ** outShader, ID3DBlob ** outBlob)
{
	ID3DBlob* blob = nullptr;
	Utility::CompileShaderFromFile((WCHAR*)filename.c_str(), entrypoint.c_str(), "vs_5_0", &blob);
	HandleError(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, outShader));

	if (outBlob)
		*outBlob = blob;
	else
		ReleaseCOM(blob);
}

void CompileAndCreateShader(ID3D11Device* device, std::wstring filename, std::string entrypoint, ID3D11PixelShader ** outShader, ID3DBlob ** outBlob)
{
	ID3DBlob* blob;
	Utility::CompileShaderFromFile((WCHAR*)filename.c_str(), entrypoint.c_str(), "ps_5_0", &blob);
	HandleError(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, outShader));

	if (outBlob)
		*outBlob = blob;
	else
		ReleaseCOM(blob);
}

void CompileAndCreateShader(ID3D11Device* device, std::wstring filename, std::string entrypoint, ID3D11GeometryShader ** outShader, ID3DBlob ** outBlob)
{
	ID3DBlob* blob;
	Utility::CompileShaderFromFile((WCHAR*)filename.c_str(), entrypoint.c_str(), "gs_5_0", &blob);
	HandleError(device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, outShader));

	if (outBlob)
		*outBlob = blob;
	else
		ReleaseCOM(blob);
}
