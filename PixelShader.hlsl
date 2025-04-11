cbuffer CBuf 
{
	float4 face_colors[8];
};

float4 main( uint DTid : SV_PrimitiveID ) : SV_TARGET 
{
	return face_colors[(DTid/2) % 8];
}