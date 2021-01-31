#include <Material.h>

Material::Material():mVertexShader(nullptr), mPixelShader(nullptr), mGeometryShader(nullptr), mHullShader(nullptr), mDomainShader(nullptr), mInputLayout(nullptr), mRasterizerState(nullptr)
{
}

void Material::StartMaterialPass(ID3D11DeviceContext * context)
{
	context->IASetInputLayout(mInputLayout);
	context->VSSetShader(mVertexShader->GenericShader, nullptr, 0);
	if (mVertexShader->ShaderConstantBuffers.size())
		context->VSSetConstantBuffers(0, (UINT)mVertexShader->ShaderConstantBuffers.size(), &mVertexShader->ShaderConstantBuffers[0]);
	
	if (mHullShader)
	{
		context->HSSetShader(mHullShader->GenericShader, nullptr, 0);
		if (mHullShader->ShaderConstantBuffers.size())
			context->HSSetConstantBuffers(0, (UINT)mHullShader->ShaderConstantBuffers.size(), &mHullShader->ShaderConstantBuffers[0]);
	}

	if (mDomainShader)
	{
		context->DSSetShader(mDomainShader->GenericShader, nullptr, 0);
		if (mDomainShader->ShaderConstantBuffers.size())
			context->DSSetConstantBuffers(0, (UINT)mDomainShader->ShaderConstantBuffers.size(), &mDomainShader->ShaderConstantBuffers[0]);
	}

	if (mGeometryShader)
	{
		context->GSSetShader(mGeometryShader->GenericShader, nullptr, 0);
		if (mGeometryShader->ShaderConstantBuffers.size())
			context->GSSetConstantBuffers(0, (UINT)mGeometryShader->ShaderConstantBuffers.size(), &mGeometryShader->ShaderConstantBuffers[0]);
	}

	if (mRasterizerState)
		context->RSSetState(mRasterizerState);

	
	context->PSSetShader(mPixelShader->GenericShader, nullptr, 0);
	if (mPixelShader->ShaderConstantBuffers.size())
		context->PSSetConstantBuffers(0, (UINT)mPixelShader->ShaderConstantBuffers.size(), &mPixelShader->ShaderConstantBuffers[0]);
	
}

void Material::EndMaterialPass(ID3D11DeviceContext* context)
{
	context->IASetInputLayout(nullptr);
	context->VSSetShader(nullptr, nullptr, 0);
	context->HSSetShader(nullptr, nullptr, 0);
	context->DSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->RSSetState(nullptr);
	context->PSSetShader(nullptr, nullptr, 0);
}
