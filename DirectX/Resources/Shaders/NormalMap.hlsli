cbuffer cbuff0:register(b0)
{
	float4 color1;//�F
	float4 color2;//�F
	float4 color3;//�F
	matrix world;//3D�ϊ��s��
	matrix view;//3D�ϊ��s��
	matrix projection;//3D�ϊ��s��
	float light;
	float uvPos;
};

struct VSOutput
{
	float4 svpos:SV_POSITION;
	float3 normal:NORMAL;
	float2 uv : TEXCOORD;
};