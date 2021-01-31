cbuffer GS_LINE_CB : register (b0)
{
	float3 CameraPosition;
	float  LineThickness;

};

cbuffer ProjectionMatrices : register (b1)
{

	matrix View;
	matrix Projection;
}

cbuffer WorldTranslation : register (b2)
{
	matrix WorldTranslationMatrix;
}

cbuffer WorldRotation : register (b3)
{
	matrix WorldRotationMatrix;
}


struct InVertex
{
	float4 pos : SV_POSITION;
	float4 color: COLOR;
};

struct OutVertex
{
	float4 pos : SV_POSITION;
	float4 color: COLOR;
};

[maxvertexcount(6)]
void gs_line(line InVertex input[2], inout TriangleStream<OutVertex> output)
{

	float4 inVertex1Pos = input[0].pos;
	float4 inVertex2Pos = input[1].pos;



	inVertex1Pos = mul(inVertex1Pos, WorldRotationMatrix);
	inVertex2Pos = mul(inVertex2Pos, WorldRotationMatrix);

	inVertex1Pos = mul(inVertex1Pos, WorldTranslationMatrix);
	inVertex2Pos = mul(inVertex2Pos, WorldTranslationMatrix);


	//get the vector that the line represents
	float3 lineVec = normalize(inVertex1Pos.xyz - inVertex2Pos.xyz);

	//get the vector looking at camera
	float3 lineCenter = (inVertex1Pos.xyz + inVertex2Pos.xyz) / 2.0f;
	float3 towardsCamVec = normalize(CameraPosition - lineCenter);

	//get the vector that is the up direction from our view point (the normal of the plane made by line and towardsCamVec)
	float4 localUpVec;
	localUpVec.xyz = normalize(cross(lineVec, towardsCamVec));
	localUpVec.w = 0.0f;

	//compose our line out of two triangles
	OutVertex v[6];
	v[0].pos = inVertex1Pos + LineThickness * localUpVec;
	v[0].color = input[0].color;
	v[1].pos = inVertex2Pos + LineThickness * localUpVec;
	v[1].color = input[1].color;
	v[2].pos = inVertex1Pos - LineThickness * localUpVec;
	v[2].color = input[0].color;
	v[3].pos = inVertex1Pos - LineThickness * localUpVec;
	v[3].color = input[0].color;
	v[4].pos = inVertex2Pos + LineThickness * localUpVec;
	v[4].color = input[1].color;
	v[5].pos = inVertex2Pos - LineThickness * localUpVec;
	v[5].color = input[1].color;


	//move to clip space 
	for (int i = 0; i < 6; i++)
	{
		v[i].pos = mul(v[i].pos, View);
		v[i].pos = mul(v[i].pos, Projection);

	}

	//output all the vertices upstream
	output.Append(v[0]);
	output.Append(v[1]);
	output.Append(v[2]);
	output.RestartStrip();
	output.Append(v[3]);
	output.Append(v[4]);
	output.Append(v[5]);
	output.RestartStrip();
}