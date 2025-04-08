cbuffer CBuf {
	matrix transformation_matrix;
}

float4 main( float3 pos : POSITION ) : SV_POSITION
{
	return mul(float4(pos, 1.0f), transformation_matrix);
}