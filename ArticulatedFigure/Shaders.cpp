#include "Shaders.h"
#include <Utility.h>
#include <Vertex.h>



namespace Shaders
{

	namespace ConstantBuffers
	{
		namespace Common
		{
			ID3D11Buffer* ViewProjectionBuffer = nullptr;
			void SetViewProjectionMatrix(ID3D11DeviceContext* context, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
			{
				ProjectionVariables pv;
				pv.View = XMMatrixTranspose(view);
				pv.Projection = XMMatrixTranspose(proj);

				context->UpdateSubresource(ViewProjectionBuffer, 0, NULL, &pv, 0, 0);
			}
		};

		namespace SimpleMaterial
		{
			ID3D11Buffer* WorldTransformBuffer = nullptr;
		};

		namespace LineMaterial
		{
			ID3D11Buffer* WorldTranslationBuffer = nullptr;
			ID3D11Buffer* WorldRotationBuffer = nullptr;

			ID3D11Buffer* LinePropertiesBuffer = nullptr;
		};

	};

	VertexShader* SimpleVertexShader = nullptr;
	VertexShader* LineVertexShader = nullptr;
	PixelShader* LinePixelShader = nullptr;
	PixelShader*  TexturePixelShader = nullptr;
	GeometryShader* LineGeometryShader = nullptr;

	ID3D11InputLayout* CommonInputLayout = nullptr;
	ID3D11InputLayout* LineInputLayout = nullptr;

	D3D11_INPUT_ELEMENT_DESC* VertexLayoutDescription = nullptr;
	D3D11_INPUT_ELEMENT_DESC* LineVertexLayoutDescription = nullptr;

	void CreateConstantBuffers(ID3D11Device* device)
	{
		ReleaseCOM(ConstantBuffers::Common::ViewProjectionBuffer);
		ReleaseCOM(ConstantBuffers::LineMaterial::WorldTranslationBuffer);
		ReleaseCOM(ConstantBuffers::LineMaterial::WorldRotationBuffer);
		ReleaseCOM(ConstantBuffers::SimpleMaterial::WorldTransformBuffer);
		ReleaseCOM(ConstantBuffers::LineMaterial::LinePropertiesBuffer);


		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(ConstantBuffers::ProjectionVariables);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		device->CreateBuffer(&bufferDesc, NULL, &ConstantBuffers::Common::ViewProjectionBuffer);

		bufferDesc.ByteWidth = sizeof(ConstantBuffers::WorldTransform);
		device->CreateBuffer(&bufferDesc, NULL, &ConstantBuffers::LineMaterial::WorldTranslationBuffer);

		bufferDesc.ByteWidth = sizeof(ConstantBuffers::WorldTransform);
		device->CreateBuffer(&bufferDesc, NULL, &ConstantBuffers::LineMaterial::WorldRotationBuffer);


		bufferDesc.ByteWidth = sizeof(ConstantBuffers::LineRendererVariables);
		device->CreateBuffer(&bufferDesc, NULL, &ConstantBuffers::LineMaterial::LinePropertiesBuffer);


		bufferDesc.ByteWidth = sizeof(ConstantBuffers::WorldTransform);
		device->CreateBuffer(&bufferDesc, NULL, &ConstantBuffers::SimpleMaterial::WorldTransformBuffer);

	
	}

	void CreateShadersAndInputLayouts(ID3D11Device* device)
	{
		ID3DBlob* vs_blob;
		ID3DBlob* line_vs_blob;

		SimpleVertexShader = new VertexShader;
		LineVertexShader = new VertexShader;
		LinePixelShader = new PixelShader;
		TexturePixelShader = new PixelShader;
		LineGeometryShader = new GeometryShader;


		CompileAndCreateShader(device, L"vs_transform.hlsl", "vs_transform", &SimpleVertexShader->GenericShader, &vs_blob);
		CompileAndCreateShader(device, L"ps_texture.hlsl", "ps_texture", &TexturePixelShader->GenericShader);
		CompileAndCreateShader(device, L"gs_line.hlsl", "gs_line", &LineGeometryShader->GenericShader);
		CompileAndCreateShader(device, L"vs_line.hlsl", "vs_line", &LineVertexShader->GenericShader, &line_vs_blob);
		CompileAndCreateShader(device, L"ps_line.hlsl", "ps_line", &LinePixelShader->GenericShader);

		CreateConstantBuffers(device);

		//set individual buffers
		SimpleVertexShader->ShaderConstantBuffers.push_back(ConstantBuffers::Common::ViewProjectionBuffer);
		SimpleVertexShader->ShaderConstantBuffers.push_back(ConstantBuffers::SimpleMaterial::WorldTransformBuffer);

		LineGeometryShader->ShaderConstantBuffers.push_back(ConstantBuffers::LineMaterial::LinePropertiesBuffer);
		LineGeometryShader->ShaderConstantBuffers.push_back(ConstantBuffers::Common::ViewProjectionBuffer);
		LineGeometryShader->ShaderConstantBuffers.push_back(ConstantBuffers::LineMaterial::WorldTranslationBuffer);
		LineGeometryShader->ShaderConstantBuffers.push_back(ConstantBuffers::LineMaterial::WorldRotationBuffer);

		
		VertexLayoutDescription = new D3D11_INPUT_ELEMENT_DESC[4];
		VertexLayoutDescription[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		VertexLayoutDescription[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		VertexLayoutDescription[2] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		VertexLayoutDescription[3] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 };

		LineVertexLayoutDescription = new D3D11_INPUT_ELEMENT_DESC[2];
		LineVertexLayoutDescription[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		LineVertexLayoutDescription[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };

		HandleError(device->CreateInputLayout(VertexLayoutDescription, NUM_MESH_VERTEX_ELEMENTS, vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), &CommonInputLayout));
		ReleaseCOM(vs_blob);

		HandleError(device->CreateInputLayout(LineVertexLayoutDescription, NUM_LINE_VERTEX_ELEMENTS, line_vs_blob->GetBufferPointer(), line_vs_blob->GetBufferSize(), &LineInputLayout));
		ReleaseCOM(line_vs_blob);

	}




};