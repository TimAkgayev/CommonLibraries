
cbuffer ProjectionMatrices : register (b0)
{

	matrix View;
	matrix Projection;
}

cbuffer WorldMatrices : register (b1)
{
	matrix World;
}

struct Vertex
{
	float4 Pos : SV_POSITION;
	float3 Norm: NORMAL;
	float4 Color : COLOR;
	float2 Uv: TEXCOORD;
};

Vertex vs_transform(float4 Pos: POSITION, float3 Normal : NORMAL, float4 Color : COLOR, float2 Uv : TEXCOORD)
{
	Vertex vs_out;
	vs_out.Pos = mul(Pos, World);
	vs_out.Pos = mul(vs_out.Pos, View);
	vs_out.Pos = mul(vs_out.Pos, Projection);
	
	vs_out.Norm = Normal;
	vs_out.Color = Color;
	vs_out.Uv = Uv;

	return vs_out;
}


