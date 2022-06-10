#include "DrawLine.h"
#include "WindowApp.h"
#include "DirectXCommon.h"
#include <cassert>
#include <DirectXTex.h>

using namespace DirectX;
using namespace Microsoft::WRL;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
ID3D12Device* DrawLine::device = nullptr;
ID3D12GraphicsCommandList* DrawLine::cmdList = nullptr;
std::unique_ptr<GraphicsPipelineManager> DrawLine::pipeline;
XMMATRIX DrawLine::matProjection;

const float PI = 3.141592f;

DrawLine::~DrawLine()
{
	vertBuff.Reset();
	constBuff.Reset();
}

void DrawLine::Finalize()
{
	pipeline.reset();
}

bool DrawLine::StaticInitialize(ID3D12Device* device)
{
	// �������`�F�b�N
	assert(!DrawLine::device);

	// nullptr�`�F�b�N
	assert(device);

	DrawLine::device = device;

	// �O���t�B�b�N�p�C�v���C���̐���
	CreateGraphicsPipeline();

	// �ˉe�s��v�Z
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)WindowApp::GetWindowWidth(),
		(float)WindowApp::GetWindowHeight(), 0.0f,
		0.0f, 1.0f);

	return true;
}

void DrawLine::CreateGraphicsPipeline()
{
	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	pipeline = GraphicsPipelineManager::Create("DrawLine2d",
		GraphicsPipelineManager::OBJECT_KINDS::DRAW_LINE_2D, inputLayout, _countof(inputLayout));
}

void DrawLine::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// PreDraw��PostDraw���y�A�ŌĂ΂�Ă��Ȃ���΃G���[
	assert(DrawLine::cmdList == nullptr);

	DrawLine::cmdList = cmdList;

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipeline->pipelineState.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(pipeline->rootSignature.Get());
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void DrawLine::PostDraw()
{
	// �R�}���h���X�g������
	DrawLine::cmdList = nullptr;
}

std::unique_ptr<DrawLine> DrawLine::Create()
{
	// Sprite�̃C���X�^���X�𐶐�
	DrawLine* instance = new DrawLine();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
		return nullptr;
	}

	instance->Update();

	return std::unique_ptr<DrawLine>(instance);
}

bool DrawLine::Initialize()
{
	// nullptr�`�F�b�N
	assert(device);

	HRESULT result = S_FALSE;

	// ���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPos) * vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	SetLine({}, {}, {}, 0);

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPos) * 4;
	vbView.StrideInBytes = sizeof(VertexPos);

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = color;
		constMap->mat = matProjection;
		constBuff->Unmap(0, nullptr);
	}

	constBuff->SetName(L"texBuff");


	return true;
}

float DrawLine::GetAngle(XMFLOAT2 startPoint, XMFLOAT2 endPoint) {
	float angle = atan2f(endPoint.y - startPoint.y, endPoint.x - startPoint.x) * (180.0f / PI);
	return angle;
}

void DrawLine::SetLine(XMFLOAT2 startPoint, XMFLOAT2 endPoint, XMFLOAT4 color, float width)
{
	HRESULT result;

	this->color = color;

	//��
	XMFLOAT2 lineWidth1 = {};
	XMFLOAT2 lineWidth2 = {};
	const float angle = GetAngle(startPoint, endPoint);
	lineWidth1.x = width * cosf((angle + 90.0f) * (PI / 180.0f));
	lineWidth1.y = width * sinf((angle + 90.0f) * (PI / 180.0f));
	lineWidth2.x = width * cosf((angle - 90.0f) * (PI / 180.0f));
	lineWidth2.y = width * sinf((angle - 90.0f) * (PI / 180.0f));

	// ���_�f�[�^
	VertexPos vertices[vertNum];

	vertices[0].pos = { startPoint.x + lineWidth2.x, startPoint.y + lineWidth2.y, 0.0f }; // ����
	vertices[1].pos = { endPoint.x + lineWidth2.x, endPoint.y + lineWidth2.y, 0.0f }; // ����
	vertices[2].pos = { startPoint.x + lineWidth1.x, startPoint.y + lineWidth1.y, 0.0f }; // �E��
	vertices[3].pos = { endPoint.x + lineWidth1.x, endPoint.y + lineWidth1.y, 0.0f }; // �E��

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}
}

void DrawLine::Update()
{
	// ���[���h�s��̍X�V
	this->matWorld = XMMatrixIdentity();
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(0.0f));
	this->matWorld *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = this->color;
		constMap->mat = this->matWorld * matProjection;	// �s��̍���	
		this->constBuff->Unmap(0, nullptr);
	}
}

void DrawLine::Draw()
{
	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());

	// �`��R�}���h
	cmdList->DrawInstanced(4, 1, 0, 0);
}