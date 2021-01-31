#pragma once 

#include <string>
#include <vector>
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <Timer.h>
#include <WindowsApplication.h>
#include <ObjFileParser.h>

using namespace DirectX;


class DirectXApplication : public WindowsApplication
{
public:
	DirectXApplication();
	DirectXApplication(const DirectXApplication& rhs) = delete;
	DirectXApplication& operator=(const DirectXApplication& rhs) = delete;
	virtual ~DirectXApplication();

protected:

	void mCreateSwapChainAndDevice();
	void mCreateRenderTargetsAndDepthBuffers();
	void mCreateViewport();

protected:

	//Device
	ID3D11Device*		 mDevice;
	ID3D11DeviceContext* mContext;
	IDXGISwapChain*		 mSwapChain;

	//Output Merger Stage
	ID3D11RenderTargetView*     mBackBufferView;
	ID3D11Texture2D* mDepthStencilTexture;
	ID3D11DepthStencilView* mDepthStencilView;
	ID3D11DepthStencilState* mDepthStencilState;

	//Rasterizer Stage
	D3D11_VIEWPORT mViewport;


};



