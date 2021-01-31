#include <Utility.h>
#include <d3dcompiler.h>

std::string Utility::GetBaseDir(const std::string & filepath)
{
	if (filepath.find_last_of("/\\") != std::wstring::npos)
		return filepath.substr(0, filepath.find_last_of("/\\"));
	return "";
}

std::wstring Utility::StringToWString(const std::string & filepath)
{
 	return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(filepath);

}

HRESULT Utility::CompileShaderFromFile(WCHAR * szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob ** ppBlobOut)
{
	
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PREFER_FLOW_CONTROL;
#endif



	ID3DBlob* errors;

	hr = D3DCompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &errors);

	if (errors)
	{
		
		OutputDebugStringA(reinterpret_cast<const char*>(errors->GetBufferPointer()));
		errors->Release();
	}


	return hr;
	
}

DirectX::XMVECTOR Utility::Float3ToVector(float x, float y, float z)
{
	DirectX::XMFLOAT3 in(x, y, z);
	return DirectX::XMLoadFloat3(&in);
}
