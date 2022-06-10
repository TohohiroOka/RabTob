#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>

#include "GraphicsPipelineManager.h"

class Camera;

class DrawLine3D
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://�\���̐錾
	//�萔�o�b�t�@�̍\����
	struct ConstBufferData {
		XMFLOAT4 color;//�F
		XMMATRIX matWorld;//3D�ϊ��s��
		XMMATRIX matView;//3D�ϊ��s��
		XMMATRIX maProjection;//3D�ϊ��s��
	};

	//���_�f�[�^3D
	struct Vertex {
		XMFLOAT3 pos;
	};

private://�ÓI�����o�֐�

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	static void CreateGraphicsPipeline();

	/// <summary>
	/// �p�x�����߂�
	/// </summary>
	/// <param name="startPoint">�n�_</param>
	/// <param name="endPoint">�I�_</param>
	/// <returns>��_�Ԃ̊p�x</returns>
	float GetAngle(XMFLOAT3 startPoint, XMFLOAT3 endPoint);

public://�ÓI�����o�֐�

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <returns></returns>
	DrawLine3D() {};

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~DrawLine3D();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// Object�̐���
	/// </summary>
	/// <param name="LineNum">���̖{��</param>
	static std::unique_ptr<DrawLine3D> Create(UINT LineNum);

	/// <summary>
	/// �J�����̃Z�b�g
	/// </summary>
	/// <param name="camera">�J����</param>
	static void SetCamera(Camera* camera) {
		DrawLine3D::camera = camera;
	}

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// �ÓI�I������
	/// </summary>
	static void Finalize();

public://�����o�֐�

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(UINT LineNum);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// ���̏��Z�b�g
	/// </summary>
	/// <param name="startPoint">�n�_</param>
	/// <param name="endPoint">�I�_</param>
	/// <param name="width">���̕�</param>
	void SetLine(XMFLOAT3 startPoint[], XMFLOAT3 endPoint[], float width);

	/// <summary>
	/// �F�̕ύX
	/// </summary>
	void SetColor(XMFLOAT4 color) { this->color = color; }

	/// <summary>
	/// //�`��
	/// </summary>
	void Draw();


private://�ÓI�����o�ϐ�

	// ��{���̒��_��
	static const int vertNum = 4;
	// ��{���̃C���f�b�N�X��
	static const int indexNum = 12;
	// ��{���̃C���f�b�NBasics
	unsigned short BASE_INDICES[indexNum] = {
	0,1,2,1,2,3,
	2,1,0,3,2,1
	};
	//�f�o�C�X
	static ID3D12Device* device;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//�J����
	static Camera* camera;
	//�p�C�v���C��
	static std::unique_ptr<GraphicsPipelineManager> pipeline;

protected://�����o�ϐ�

	//���_�z��
	std::vector<Vertex> vertices;
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//���_�f�[�^�̗v�f��
	UINT VERTEX_ARRAY_NUM = 0;
	//�C���f�b�N�X�f�[�^�̗v�f��
	UINT INDEX_ARRAY_NUM = 0;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// �F
	XMFLOAT4 color = {};
};