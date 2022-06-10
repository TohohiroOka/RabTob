#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>

#include "GraphicsPipelineManager.h"

class DrawLine
{
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	/// <summary>
	/// ���_�f�[�^�\����
	/// </summary>
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz���W
	};

	/// <summary>
	/// �萔�o�b�t�@�p�f�[�^�\����
	/// </summary>
	struct ConstBufferData
	{
		XMFLOAT4 color;	// �F (RGBA)
		XMMATRIX mat;	// �R�c�ϊ��s��
	};

private: // �ÓI�����o�֐�

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C���̐���
	/// </summary>
	static void CreateGraphicsPipeline();

public: // �ÓI�����o�֐�

	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <returns>����</returns>
	static bool StaticInitialize(ID3D12Device* device);

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
	/// �X�v���C�g����
	/// </summary>
	static std::unique_ptr<DrawLine> Create();

	/// <summary>
	/// �������
	/// </summary>
	static void Finalize();

protected: // �ÓI�����o�ϐ�

	// ���_��
	static const int vertNum = 4;
	// �f�o�C�X
	static ID3D12Device* device;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//�p�C�v���C��
	static std::unique_ptr<GraphicsPipelineManager> pipeline;
	// �ˉe�s��
	static XMMATRIX matProjection;

public: // �����o�֐�

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	DrawLine() {};

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~DrawLine();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize();

	/// <summary>
	/// �p�x�����߂�
	/// </summary>
	/// <param name="startPoint">�n�_</param>
	/// <param name="endPoint">�I�_</param>
	/// <returns>��_�Ԃ̊p�x</returns>
	float GetAngle(XMFLOAT2 startPoint, XMFLOAT2 endPoint);

	/// <summary>
	/// ���̏��Z�b�g
	/// </summary>
	/// <param name="startPoint">�n�_</param>
	/// <param name="endPoint">�I�_</param>
	/// <param name="color">�F</param>
	/// <param name="width">���̕�</param>
	void SetLine(DirectX::XMFLOAT2 startPoint, DirectX::XMFLOAT2 endPoint, XMFLOAT4 color, float width);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

protected: // �����o�ϐ�

	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// ���[���h�s��
	XMMATRIX matWorld{};
	// �F
	XMFLOAT4 color = { 1, 1, 1, 1 };
};