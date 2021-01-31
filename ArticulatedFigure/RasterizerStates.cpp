#include "RasterizerStates.h"


namespace RasterizerStates
{
	ID3D11RasterizerState* DefaultState = nullptr;


	void RasterizerStates::CreateRasterizerStates(ID3D11Device* device)
	{
		//Rasterizer Stage
		D3D11_RASTERIZER_DESC RasterizerStateDesc;
		RasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
		RasterizerStateDesc.CullMode = D3D11_CULL_NONE;
		RasterizerStateDesc.FrontCounterClockwise = true;
		RasterizerStateDesc.DepthBias = 0;
		RasterizerStateDesc.DepthBiasClamp = 0;
		RasterizerStateDesc.SlopeScaledDepthBias = 0;
		RasterizerStateDesc.DepthClipEnable = true;
		RasterizerStateDesc.ScissorEnable = false;
		RasterizerStateDesc.MultisampleEnable = false;
		RasterizerStateDesc.AntialiasedLineEnable = false;
		device->CreateRasterizerState(&RasterizerStateDesc, &DefaultState);

	}

};