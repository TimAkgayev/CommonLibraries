#include "Materials.h"
#include "Shaders.h"
#include "RasterizerStates.h"

LineMaterial::LineMaterial()
{
	mInputLayout = Shaders::LineInputLayout;
	mVertexShader = Shaders::LineVertexShader;
	mGeometryShader = Shaders::LineGeometryShader;
	mPixelShader = Shaders::LinePixelShader;
	mRasterizerState = RasterizerStates::DefaultState;
}



void LineMaterial::SetWorldPosition(ID3D11DeviceContext * context, DirectX::XMFLOAT3 & worldPos)
{
	Shaders::ConstantBuffers::WorldTransform w;
	w.WorldTransform = DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(worldPos.x, worldPos.y, worldPos.z));
	context->UpdateSubresource(Shaders::ConstantBuffers::LineMaterial::WorldTranslationBuffer, 0, NULL, &w, 0, 0);
}

void LineMaterial::SetWorldRotation(ID3D11DeviceContext* context, DirectX::XMMATRIX &rotation)
{

	Shaders::ConstantBuffers::WorldTransform w;
	w.WorldTransform = XMMatrixTranspose(rotation);
	context->UpdateSubresource(Shaders::ConstantBuffers::LineMaterial::WorldRotationBuffer, 0, NULL, &w, 0, 0);
}

void LineMaterial::SetLineWidthAndCameraPosition(ID3D11DeviceContext* context, float width, DirectX::XMFLOAT3 & cameraPos)
{
	Shaders::ConstantBuffers::LineRendererVariables lp;
	lp.CameraPosition = cameraPos;
	lp.LineThickness = width;

	context->UpdateSubresource(Shaders::ConstantBuffers::LineMaterial::LinePropertiesBuffer, 0, NULL, &lp, 0, 0);
}

SimpleMaterial::SimpleMaterial()
{
	mInputLayout = Shaders::CommonInputLayout;
	mVertexShader = Shaders::SimpleVertexShader;
	mPixelShader = Shaders::TexturePixelShader;
	mRasterizerState = RasterizerStates::DefaultState;

}

void SimpleMaterial::SetWorldPosition(ID3D11DeviceContext* context, DirectX::XMMATRIX & world)
{
	Shaders::ConstantBuffers::WorldTransform w;
	w.WorldTransform = XMMatrixTranspose(world);
	context->UpdateSubresource(Shaders::ConstantBuffers::SimpleMaterial::WorldTransformBuffer, 0, NULL, &w, 0, 0);
}
