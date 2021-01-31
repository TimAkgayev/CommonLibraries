#pragma once
#include <d3d11.h>
#include <string>
#include <vector>
#include <GenericShader.h>
#include <DirectXMath.h>

namespace Shaders
{
	
	namespace ConstantBuffers
	{
		
		//constant buffers
		struct ProjectionVariables
		{

			DirectX::XMMATRIX View;
			DirectX::XMMATRIX Projection;

		};

		struct WorldTransform
		{
			DirectX::XMMATRIX WorldTransform;
		};


		struct LineRendererVariables
		{
			DirectX::XMFLOAT3 CameraPosition;
			float LineThickness;
		};

		namespace Common
		{
			extern ID3D11Buffer* ViewProjectionBuffer;
			void SetViewProjectionMatrix(ID3D11DeviceContext*, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj);
		};

		namespace SimpleMaterial
		{
			extern ID3D11Buffer* WorldTransformBuffer;
		};

		namespace LineMaterial
		{
			extern ID3D11Buffer* WorldTranslationBuffer;
			extern ID3D11Buffer* WorldRotationBuffer;
			extern ID3D11Buffer* LinePropertiesBuffer;
		};
	};


	//Shader collection
	extern VertexShader* LineVertexShader;
	extern VertexShader* SimpleVertexShader;
	extern PixelShader* LinePixelShader;
	extern PixelShader*  TexturePixelShader;
	extern GeometryShader* LineGeometryShader;

	extern ID3D11InputLayout* CommonInputLayout;
	extern ID3D11InputLayout* LineInputLayout;

	extern D3D11_INPUT_ELEMENT_DESC* VertexLayoutDescription;
	extern D3D11_INPUT_ELEMENT_DESC* LineVertexLayoutDescription;

	void CreateShadersAndInputLayouts(ID3D11Device* device);
};