#include "DrawLine3D.hlsli"

VSOutput main( float4 pos : POSITION ) : SV_POSITION
{
	//�s�N�Z���V�F�[�_�[�ɓn���l
	VSOutput output;

	//�s���pos�ɔ��f
	output.svpos = mul(world, pos);
	output.svpos = mul(view, output.svpos);
	output.svpos = mul(projection, output.svpos);

	return output;
}