cbuffer cbuff0:register(b0)
{
	float4 color;//�F
	matrix world;//3D�ϊ��s��
	matrix view;//3D�ϊ��s��
	matrix projection;//3D�ϊ��s��
};

struct VSOutput
{
	float4 svpos:SV_POSITION;
};