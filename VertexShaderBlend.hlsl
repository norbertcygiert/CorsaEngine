cbuffer CBuf 
{
	matrix transformation_matrix;
};

struct VSOUT 
{
	float4 color : COLOR;
	float4 pos : SV_POSITION;
};

VSOUT main( float3 pos : POSITION, float4 color : COLOR )
{
	VSOUT vso;
	vso.pos = mul(float4(pos, 1.0f), transformation_matrix);
	vso.color = color;
	return vso;
}