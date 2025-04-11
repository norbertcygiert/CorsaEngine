cbuffer CBUF {
	matrix transformation_matrix;
};

struct VSOUT {
	float2 tex : TEXCOORD;
	float4 pos : SV_POSITION;
};

VSOUT main( float3 pos : POSITION, float2 tex : TEXCOORD )
{
	VSOUT vso;
	vso.pos = mul(float4(pos,1.0f), transformation_matrix);
	vso.tex = tex;
	return vso;
}