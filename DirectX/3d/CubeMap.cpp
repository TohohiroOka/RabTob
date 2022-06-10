#include "CubeMap.h"
#include "Camera.h"
#include <DirectXTex.h>
#include <string>
#include "SafeDelete.h"

using namespace Microsoft::WRL;
using namespace DirectX;

ID3D12Device* CubeMap::device = nullptr;
ID3D12GraphicsCommandList* CubeMap::cmdList = nullptr;
std::unique_ptr<GraphicsPipelineManager> CubeMap::pipeline = nullptr;
Camera* CubeMap::camera = nullptr;

void CubeMap::CreateGraphicsPipeline()
{
	////���_���C�A�E�g�z��̐錾�Ɛݒ�
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT,
			0,D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	pipeline = GraphicsPipelineManager::Create("CubeBox",
		GraphicsPipelineManager::OBJECT_KINDS::CUBE_BOX, inputLayout, _countof(inputLayout));
}

void CubeMap::StaticInitialize(ID3D12Device* device)
{
	// �ď������`�F�b�N
	assert(!CubeMap::device);
	assert(device);
	CubeMap::device = device;

	CreateGraphicsPipeline();
}

std::unique_ptr<CubeMap> CubeMap::Create(ID3D12GraphicsCommandList* cmdList)
{
	//�C���X�^���X�𐶐�
	CubeMap* instance = new CubeMap();
	if (instance == nullptr) {
		return nullptr;
	}

	instance->texture = Texture::Create("Resources/CubeMap/cubemap.dds", cmdList);

	//������
	instance->Initialize();

	return std::unique_ptr<CubeMap>(instance);
}

void CubeMap::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// PreDraw��PostDraw���y�A�ŌĂ΂�Ă��Ȃ���΃G���[
	assert(CubeMap::cmdList == nullptr);

	CubeMap::cmdList = cmdList;

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipeline->pipelineState.Get());

	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(pipeline->rootSignature.Get());

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void CubeMap::PostDraw()
{
	// �R�}���h���X�g������
	CubeMap::cmdList = nullptr;
}

void CubeMap::Finalize()
{
	pipeline.reset();
}

void CubeMap::Initialize()
{
	HRESULT result = S_FALSE;
	//���_�̒���
	float edge = 1.0f;
	const int vertNum = 24;
	Vertex vertices[vertNum] = {
		//face1
		{{ -edge, -edge, -edge }, { -1.0f, -1.0f, -1.0f}}, // ���� / ���_1
		{{ -edge,  edge, -edge }, { -1.0f,  1.0f, -1.0f}}, // ���� / ���_2
		{{  edge, -edge, -edge }, { -1.0f, -1.0f,  1.0f}}, // �E�� / ���_3
		{{  edge,  edge, -edge }, { -1.0f,  1.0f,  1.0f}}, // �E�� / ���_4
		//face0
		{{  edge, -edge,  edge }, {  1.0f, -1.0f,  1.0f}}, // ���� / ���_5
		{{  edge,  edge,  edge }, {  1.0f,  1.0f,  1.0f}}, // ���� / ���_6
		{{ -edge, -edge,  edge }, {  1.0f, -1.0f, -1.0f}}, // �E�� / ���_7
		{{ -edge,  edge,  edge }, {  1.0f,  1.0f, -1.0f}}, // �E�� / ���_8
		//face4
		{{  edge, -edge, -edge }, { -1.0f, -1.0f,  1.0f}}, // ���� / ���_3
		{{  edge,  edge, -edge }, { -1.0f,  1.0f,  1.0f}}, // ���� / ���_4
		{{  edge, -edge,  edge }, {  1.0f, -1.0f,  1.0f}}, // �E�� / ���_5
		{{  edge,  edge,  edge }, {  1.0f,  1.0f,  1.0f}}, // �E�� / ���_6
		//face5
		{{ -edge, -edge,  edge }, {  1.0f, -1.0f, -1.0f}}, // ���� / ���_3
		{{ -edge,  edge,  edge }, {  1.0f,  1.0f, -1.0f}}, // ���� / ���_4
		{{ -edge, -edge, -edge }, { -1.0f, -1.0f, -1.0f}}, // �E�� / ���_1
		{{ -edge,  edge, -edge }, { -1.0f,  1.0f, -1.0f}}, // �E�� / ���_2
		//face2
		{{  edge,  edge, -edge }, { -1.0f,  1.0f,  1.0f}}, // ���� / ���_4
		{{ -edge,  edge, -edge }, { -1.0f,  1.0f, -1.0f}}, // ���� / ���_2
		{{  edge,  edge,  edge }, {  1.0f,  1.0f,  1.0f}}, // �E�� / ���_6
		{{ -edge,  edge,  edge }, {  1.0f,  1.0f, -1.0f}}, // �E�� / ���_8
		//face3
		{{ -edge, -edge, -edge }, { -1.0f, -1.0f, -1.0f}}, // ���� / ���_1
		{{  edge, -edge, -edge }, { -1.0f, -1.0f,  1.0f}}, // ���� / ���_3
		{{ -edge, -edge,  edge }, {  1.0f, -1.0f, -1.0f}}, // �E�� / ���_7
		{{  edge, -edge,  edge }, {  1.0f, -1.0f,  1.0f}}, // �E�� / ���_5
	};

	//���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex) * vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	vertBuff->Unmap(0, nullptr);

	//���_�o�b�t�@�r���[�̐���
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(Vertex) * vertNum;
	vbView.StrideInBytes = sizeof(Vertex);

	unsigned short indices[indexNum] = {
		0 ,2 ,1 ,2 ,3 ,1 ,
		4 ,6 ,5 ,6 ,7 ,5 ,
		8 ,10,9 ,10,11,9 ,
		12,14,13,14,15,13,
		16,18,17,18,19,17,
		20,22,21,22,23,21
	};

	//�C���f�b�N�X�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(unsigned short) * indexNum), // ���\�[�X�ݒ�
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
	ibView.SizeInBytes = sizeof(unsigned short) * indexNum;

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

CubeMap::~CubeMap()
{
	vertBuff.Reset();
	indexBuff.Reset();
	texConstBuffer.Reset();
	constBuff.Reset();
	texture.reset();
}

void CubeMap::Update()
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

	const XMMATRIX& matViewProjection = camera->GetView() * camera->GetProjection();
	const XMFLOAT3& cameraPos = camera->GetEye();

	//�萔�o�b�t�@�Ƀf�[�^��]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
	assert(SUCCEEDED(result));
	constMap->viewproj = matViewProjection;
	constMap->world = matWorld;
	constMap->cameraPos = cameraPos;
	constBuff->Unmap(0, nullptr);
}

void CubeMap::Draw()
{
	//�萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//���_�o�b�t�@�̐ݒ�
	cmdList->IASetIndexBuffer(&ibView);

	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//�e�N�X�`���o�b�t�@�]��
	TransferTextureBubber(cmdList, 1);

	//�`��R�}���h
	cmdList->DrawIndexedInstanced(indexNum, 1, 0, 0, 0);
}

void CubeMap::TransferTextureBubber(ID3D12GraphicsCommandList* cmdList,UINT RootParameterIndex)
{
	cmdList->SetGraphicsRootDescriptorTable(RootParameterIndex, texture->descriptor->gpu);
}