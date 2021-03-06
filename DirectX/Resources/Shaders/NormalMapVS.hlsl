#include "NormalMap.hlsli"

VSOutput main(float4 pos:POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VSOutput output;//sNZVF[_[Én·l
	////sñðposÉ½f
	output.svpos = mul(world, pos);
	output.svpos = mul(view, output.svpos);
	output.svpos = mul(projection, output.svpos);

	//uvð½f
	output.uv = uv;

	//@üÆsñÌvZ
	output.normal = mul(world, normal);
	output.normal = normalize(output.normal);

	return output;
}