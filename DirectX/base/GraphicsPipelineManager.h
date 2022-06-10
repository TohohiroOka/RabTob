#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include "ShaderManager.h"

class GraphicsPipelineManager
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public://�����oenum

	enum OBJECT_KINDS
	{
		NONE,
		OBJ,
		SPRITE,
		FBX,
		DRAW_LINE_2D,
		DRAW_LINE_3D,
		PARTICLE,
		NORMAL_MAP,
		POST_EFFECT,
		CUBE_BOX,
	};

private://�����o�֐�

	/// <summary>
	/// �O���t�B�b�N�X�p�C�v���C���̊�Ղ̐���
	/// </summary>
	static void CreateDefaultGpipeline();

	/// <summary>
	/// �p�C�v���C���f�X�N�̐���
	/// </summary>
	/// <param name="inputLayout">���_���C�A�E�g</param>
	/// <param name="inputLayoutSize">���_���C�A�E�g�̃T�C�Y</param>
	/// <returns>�p�C�v���C���f�X�N</returns>
	D3D12_GRAPHICS_PIPELINE_STATE_DESC CreatepelineDesc(
		const D3D12_INPUT_ELEMENT_DESC* inputLayout, const UINT inputLayoutSize);

	/// <summary>
	/// ���[�g�V�O�l�`���̐���
	/// </summary>
	void CreateRootSignature();

public://�����o�֐�

	GraphicsPipelineManager() {};
	~GraphicsPipelineManager();

	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	/// <param name="className">�Ăяo���N���X�̖��O</param>
	/// <param name="objectKind">�I�u�W�F�N�g�̎��</param>
	/// <param name="inputLayout">���_���C�A�E�g</param>
	/// <param name="inputLayoutSize">���_���C�A�E�g�̃T�C�Y</param>
	/// <returns>�C���X�^���X</returns>
	static std::unique_ptr<GraphicsPipelineManager> Create(const std::string name, const unsigned char objectKind,
		const D3D12_INPUT_ELEMENT_DESC* inputLayout, const UINT inputLayoutSize);

	/// <summary>
	/// �p�C�v���C���̐���
	/// enum OBJECT_KINDS�̒������������Ƃ���
	/// </summary>
	/// <param name="className">�Ăяo���N���X�̖��O</param>
	/// <param name="objectKind">�I�u�W�F�N�g�̎��</param>
	/// <param name="inputLayout">���_���C�A�E�g</param>
	/// <param name="inputLayoutSize">���_���C�A�E�g�̃T�C�Y</param>
	void CreatePipeline(const std::string name, const unsigned char objectKind,
		const D3D12_INPUT_ELEMENT_DESC* inputLayout, const UINT inputLayoutSize);

	/// <summary>
	/// �������
	/// </summary>
	static void Finalize();

private://�����o�ϐ�

	// �f�o�C�X
	static ID3D12Device* device;
	//�V�F�[�_�[
	static std::unique_ptr<ShaderManager> shaderManager;
	//�����I�u�W�F�N�g�̎��
	unsigned char objectKind = OBJECT_KINDS::NONE;
	//�O���t�B�b�N�X�p�C�v���C���̊��
	static D3D12_GRAPHICS_PIPELINE_STATE_DESC defaultGpipeline;
	//�u�����h�ݒ�̊��
	static D3D12_RENDER_TARGET_BLEND_DESC blenddesc;

public://�����o�ϐ�

	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	ComPtr<ID3D12PipelineState> pipelineState = nullptr;
};
