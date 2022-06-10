#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
private://�ÓI�����o�֐�

	/// <summary>
	/// �O���t�B�b�N�X�p�C�v���C���̐���
	/// </summary>
	static void CreateGraphicsPipeline();

public: // �T�u�N���X

	// �萔�o�b�t�@�p�f�[�^�\����B0
	struct ConstBufferData
	{
		XMFLOAT4 outlineColor;//�A�E�g���C���̐F
		float outlineWidth;//�A�E�g���C���̑���
	};

public://�����o�֐�

	/// <summary>
	/// �ÓI������
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	static std::unique_ptr<PostEffect> Create();

	/// <summary>
	/// �������
	/// </summary>
	void Finalize();

public://�����o�֐�

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PostEffect();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~PostEffect();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

private://�ÓI�����o�ϐ�
	
	//�e�N�X�`���o�b�t�@�̐�
	static const int texBuffNum = 3;
	//��ʃN���A�J���[
	static const float clearColor[texBuffNum][4];
	//�e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource> texBuff[texBuffNum];
	//�p�C�v���C��
	static std::unique_ptr<GraphicsPipelineManager> pipeline;

private://�����o�ϐ�

	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> depthBuff;
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
};