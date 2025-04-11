Texture2D tex;

SamplerState s;

float4 main( float2 tex_pos : TEXCOORD ) : SV_TARGET
{
	return tex.Sample(s,tex_pos);
}