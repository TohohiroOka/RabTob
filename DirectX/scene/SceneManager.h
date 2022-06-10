#pragma once
#include "InterfaceScene.h"

class SceneManager
{
public://�ÓI�����o�֐�

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<SceneManager> Create();

public://�����o�֐�

	SceneManager() {};
	~SceneManager();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// imgui�̕\��
	/// </summary>
	void ImguiDraw();

	/// <summary>
	/// �R���s���[�g�V�F�[�_�[����̌v�Z���ʎ擾
	/// </summary>
	void GetConstbufferNum();

	/// <summary>
	/// ���V�[���ւ̈ڍs
	/// </summary>
	/// <param name="inScene">���V�[���̃C���X�^���X</param>
	static void SetNextScene(InterfaceScene* inScene) { nextScene = inScene; }

private://�����o�ϐ�

	//���݃V�[��
	static std::unique_ptr<InterfaceScene> scene;
	//���V�[��
	static InterfaceScene* nextScene;
	//Camera�̃C���X�^���X
	std::unique_ptr<Camera> camera = nullptr;
	//�I�[�f�B�I
	std::unique_ptr<Audio> audio = nullptr;
	//���C�g
	std::unique_ptr<LightGroup> light = nullptr;
};