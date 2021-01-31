

struct Vertex
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

Vertex vs_line(float4 Pos: POSITION, float4 Color : COLOR)
{
	Vertex vs_out;
	vs_out.Pos = Pos;
	vs_out.Color = Color;


	return vs_out;
}

