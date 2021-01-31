#include <DirectXApplication.h>
#include <wrl\wrappers\corewrappers.h>
#include <Utility.h>


DirectXApplication::DirectXApplication()
{

}

DirectXApplication::~DirectXApplication()
{

}

void DirectXApplication::mCreateSwapChainAndDevice()
{

	//initialize COM interface to use the WIC imagine functions

#if (_WIN32_WINNT >= 0x0A00 /*_WIN32_WINNT_WIN10*/)
	Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
	if (FAILED(initialize))
		return;// error
#else
	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
		// error
#endif
		HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);



	//setup swap chain ===================================================
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	//set buffer dimensions and format
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = mClientWidth;
	swapChainDesc.BufferDesc.Height = mClientHeight;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;


	//set refresh rate
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	//sampling settings
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SampleDesc.Count = 1;

	//output window handle
	swapChainDesc.OutputWindow = mMainWindow;
	swapChainDesc.Windowed = true;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;


	//create device ===================================================
	UINT createDeviceFlags = 0;
#ifdef _DEBUG 
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL d3dFL;

	HandleError(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, 0, 0, D3D11_SDK_VERSION, &swapChainDesc, &mSwapChain, &mDevice, &d3dFL, NULL));
	mDevice->GetImmediateContext(&mContext);

}

void DirectXApplication::mCreateRenderTargetsAndDepthBuffers()
{
	// create render target for merger state ===================================================
	ID3D11Texture2D* pBackBuffer;
	HandleError(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));

	//try to create a view to the back buffer
	HandleError(mDevice->CreateRenderTargetView(pBackBuffer, NULL, &mBackBufferView));

	//release the back buffer
	pBackBuffer->Release();

	//create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = mClientWidth;
	descDepth.Height = mClientHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HandleError(mDevice->CreateTexture2D(&descDepth, NULL, &mDepthStencilTexture));
	HandleError(mDevice->CreateDepthStencilView(mDepthStencilTexture, 0, &mDepthStencilView));

	//set up depth-stencil buffer
	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	mDevice->CreateDepthStencilState(&dsDesc, &mDepthStencilState);
}

void DirectXApplication::mCreateViewport()
{
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = (FLOAT)mClientWidth;
	mViewport.Height = (FLOAT)mClientHeight;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1;
}
