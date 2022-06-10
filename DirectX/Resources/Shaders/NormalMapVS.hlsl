#include "NormalMap.hlsli"

VSOutput main(float4 pos:POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VSOutput output;//�s�N�Z���V�F�[�_�[�ɓn���l
	////�s���pos�ɔ��f
	output.svpos = mul(world, pos);
	output.svpos = mul(view, output.svpos);
	output.svpos = mul(projection, output.svpos);

	//uv�𔽉f
	output.uv = uv;

	//�@���ƍs��̌v�Z
	output.normal = mul(world, normal);
	output.normal = normalize(output.normal);

	return output;
}