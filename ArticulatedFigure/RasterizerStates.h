#pragma once
#include <d3d11.h>

namespace RasterizerStates
{
	extern ID3D11RasterizerState* DefaultState;
	void CreateRasterizerStates(ID3D11Device*);
};