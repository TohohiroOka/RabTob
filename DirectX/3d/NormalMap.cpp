#include "NormalMap.h"
#include "Camera.h"
#include <DirectXTex.h>
#include"Camera.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace DirectX;
using namespace Microsoft::WRL;

using namespace std;

ID3D12Device* NormalMap::device = nullptr;
ID3D12GraphicsCommandList* NormalMap::cmdList = nullptr;
Camera* NormalMap::camera = nullptr;
std::unique_ptr<GraphicsPipelineManager> NormalMap::pipeline;
ComPtr<ID3D12DescriptorHeap> NormalMap::descHeap;
ComPtr<ID3D12Resource> NormalMap::texBuffer[textureNum];
int NormalMap::alltextureNum = 0;
//���_�f�[�^
NormalMap::Vertex NormalMap::vertices[24] = {
	//�O
	{{ -edge, -edge, -edge },{},{0.0f,1.0f}}, // ����0
	{{ -edge, edge, -edge },{},{0.0f,0.0f}}, // ����1
	{{ edge, -edge, -edge },{}, { 1.0f,1.0f }}, // �E��2
	{{ edge, edge, -edge },{}, {1.0f,0.0f}}, // �E��3
	//��
	{{ -edge, edge, edge },{}, {0.0f,0.0f}}, // ����4
	{{ -edge, -edge, edge },{}, {0.0f,1.0f}}, // ����5
	{{ edge, edge, edge },{}, {1.0f,0.0f}}, // �E��6
	{{ edge, -edge, edge },{}, {1.0f,1.0f}}, // �E��7
	//��
	{{ -edge, -edge, -edge },{}, {0.0f,1.0f}}, // ����8
	{{ -edge, -edge, edge },{}, {0.0f,0.0f}}, // ����9
	{{ -edge, edge, -edge },{}, {1.0f,1.0f}}, // �E��10
	{{ -edge, edge, edge },{}, {1.0f,0.0f}}, // �E��11
	//�E
	{{ edge, -edge, edge },{}, {0.0f,1.0f}}, // ����12
	{{ edge, -edge, -edge },{}, {0.0f,0.0f}}, // ����13
	{{ edge, edge, edge },{}, {1.0f,1.0f}}, // �E��14
	{{ edge, edge, -edge },{}, {1.0f,0.0f}}, // �E��15
	//��
	{{ -edge, edge, -edge },{}, {0.0f,1.0f}}, // ����16
	{{ -edge, edge, edge },{}, {0.0f,0.0f}}, // ����17
	{{ edge, edge, -edge },{}, {1.0f,1.0f}}, // �E��18
	{{ edge, edge, edge },{}, {1.0f,0.0f}}, // �E��19
	//��
	{{ -edge, -edge, edge },{}, {0.0f,0.0f}}, // ����20
	{{ -edge, -edge, -edge },{}, {0.0f,1.0f}}, // ����21
	{{ edge, -edge, edge },{}, {1.0f,0.0f}}, // �E��22
	{{ edge, -edge, -edge },{}, {1.0f,1.0f}}, // �E��23
};
//�C���f�b�N�X�f�[�^
const unsigned short NormalMap::indices[36] = {
	//�O
	0,1,2,2,1,3,
	//��
	4,5,6,6,5,7,
	//��
	8,9,10,10,9,11,
	//�E
	12,13,14,14,13,15,
	//��
	16,17,18,18,17,19,
	//��
	20,21,22,22,21,23,
};

NormalMap::~NormalMap()
{
	vertBuff.Reset();
	indexBuff.Reset();
	constBuff.Reset();
}

void NormalMap::CreateGraphicsPipeline()
{
	////���_���C�A�E�g�z��̐錾�Ɛݒ�
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"NORMAL", 0,DXGI_FORMAT_R32G32B32_FLOAT,
			0,D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		 },
		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
			0,D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	pipeline = GraphicsPipelineManager::Create("NormalMap",
		GraphicsPipelineManager::OBJECT_KINDS::NORMAL_MAP, inputLayout, _countof(inputLayout));
}

void NormalMap::CommonCreate() {
	HRESULT result = S_FALSE;

	//�f�X�N���v�^�q�[�v�̐���
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = textureNum;
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));
}

int NormalMap::LoadTexture(const wchar_t* filename)
{
	HRESULT result;

	//�ُ�Ȕԍ��̎w��
	assert(alltextureNum <= textureNum - 1);

	////WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImage{};

	result = LoadFromWICFile(
		filename,
		WIC_FLAGS_NONE,
		&metadata, scratchImage);

	const Image* img = scratchImage.GetImage(0, 0, 0);

	//�e�N�X�`���o�b�t�@�̐���
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	//�e�N�X�`���o�b�t�@����
	result = device->CreateCommittedResource(//GPU���\�[�X�̐���
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//�e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&texBuffer[alltextureNum]));

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texBuffer[alltextureNum]->WriteToSubresource(
		0,
		nullptr,//�S�̈�փR�s�[
		img->pixels,//���f�[�^�A�h���X
		(UINT)img->rowPitch,//�P���C���T�C�Y
		(UINT)img->slicePitch//1���T�C�Y
	);

	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	//�q�[�v��texnumber�ԖڂɃV�F�[�_�[���\�[�X�r���[���쐬
	device->CreateShaderResourceView(
		texBuffer[alltextureNum].Get(),
		&srvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), alltextureNum,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//�e�N�X�`����
	alltextureNum++;

	return alltextureNum - 1;
}

void NormalMap::Initialize()
{
	HRESULT result = S_FALSE;

	//���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//�@�������̌v�Z
	for (int i = 0; i < _countof(indices) / 3; i++)
	{
		//�ꎞ�I�Ɋi�[
		unsigned short index_0 = indices[i * 3 + 0];
		unsigned short index_1 = indices[i * 3 + 1];
		unsigned short index_2 = indices[i * 3 + 2];

		//���_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[index_0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index_1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index_2].pos);

		//p0->p1,p0->p2�x�N�g�����v�Z
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//�O�ς͗������璼�p�ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);

		//���K��
		normal = XMVector3Normalize(normal);

		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[index_0].normal, normal);
		XMStoreFloat3(&vertices[index_1].normal, normal);
		XMStoreFloat3(&vertices[index_2].normal, normal);
	}

	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	vertBuff->Unmap(0, nullptr);

	//���_�o�b�t�@�r���[�̐���
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Vertex);

	//�C���f�b�N�X�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB), // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	assert(SUCCEEDED(result));

	//�C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	memcpy(indexMap, indices, sizeof(indices));
	indexBuff->Unmap(0, nullptr);

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));
}

std::unique_ptr<NormalMap> NormalMap::Create()
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	NormalMap* instance = new NormalMap();

	// ������
	instance->Initialize();

	return std::unique_ptr<NormalMap>(instance);
}

void NormalMap::StaticInitialize(ID3D12Device* device)
{
	NormalMap::device = device;

	//���ʃf�[�^����
	CommonCreate();

	//�p�C�v���C���ݒ�
	CreateGraphicsPipeline();
}

void NormalMap::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	NormalMap::cmdList = cmdList;

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipeline->pipelineState.Get());

	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(pipeline->rootSignature.Get());

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�f�X�N���v�^�q�[�v���Z�b�g
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void NormalMap::PostDraw()
{
	// �R�}���h���X�g������
	NormalMap::cmdList = nullptr;
}

void NormalMap::Update()
{
	//���[���h�s��ϊ�
	XMMATRIX matWorld = XMMatrixIdentity();
	XMMATRIX matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matWorld *= matScale;

	XMMATRIX matRot;//�p�x
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.y));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.z));
	matWorld *= matRot;

	XMMATRIX matTrans;//���s����
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	matWorld *= matTrans;

	//�萔�o�b�t�@�Ƀf�[�^��]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
	constMap->color1 = color1;
	constMap->color2 = color2;
	constMap->color3 = color3;
	constMap->matWorld = matWorld;
	constMap->matView = camera->GetView();
	constMap->maProjection = camera->GetProjection();
	constMap->light = light;
	constMap->uvPos = this->uvPos;

	constBuff->Unmap(0, nullptr);
}

void NormalMap::Draw(int colorTex, int normalTex, int normalTex2)
{
	//���_�o�b�t�@�̐ݒ�
	cmdList->IASetIndexBuffer(&ibView);

	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//�萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//�V�F�[�_�[���\�[�X�r���[���Z�b�g
	auto heapStart = descHeap->GetGPUDescriptorHandleForHeapStart();

	cmdList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			heapStart,
			colorTex,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	cmdList->SetGraphicsRootDescriptorTable(2,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			heapStart,
			normalTex,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	cmdList->SetGraphicsRootDescriptorTable(3,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			heapStart,
			normalTex2,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//�`��R�}���h
	cmdList->DrawIndexedInstanced(36, 1, 0, 0, 0);
}

void NormalMap::Finalize()
{
	descHeap.Reset();
	for (int i = 0; i < textureNum; i++)
	{
		texBuffer[i].Reset();
	}
	pipeline.reset();
}