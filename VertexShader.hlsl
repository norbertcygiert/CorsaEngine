struct VSOUT
{
	float3 color : COLOR;
	float4 pos : SV_POSITION;
};

cbuffer CBuf {
	row_major matrix transformation_matrix;
}

VSOUT main( float2 pos : POSITION, float3 color : COLOR )
{
	VSOUT vso;
	vso.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transformation_matrix);
	vso.color = color;
	return vso;
}