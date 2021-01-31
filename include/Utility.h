#pragma once
#include <string>

//for converting std::string to std::wstring
#include <locale>
#include <codecvt>

#include <d3d11.h>
#include <DirectXMath.h>

#define ReleaseCOM(x) { if(x){ x->Release(); x = nullptr; } }
#define HandleError(x)                                              \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			std::string debugText = "Function Failed at line number " + __LINE__;						\
			OutputDebugString(debugText.c_str());  \
		}                                                      \
	}

namespace Utility
{

	std::string GetBaseDir(const std::string& filepath);
	std::wstring StringToWString(const std::string& filepath);
	HRESULT CompileShaderFromFile(WCHAR * szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob ** ppBlobOut);
	DirectX::XMVECTOR Float3ToVector(float x, float y, float z);
}