Texture2D TextureMap : register(t0);
SamplerState Sampler : register(s0);

struct Vertex
{
	float4 Pos : SV_POSITION;
	float3 Norm: NORMAL;
	float4 Color : COLOR;
	float2 Uv: TEXCOORD;
};


float4 ps_texture(Vertex in_vertex) : SV_Target
{
	return TextureMap.Sample(Sampler, in_vertex.Uv);
}