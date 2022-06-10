#pragma once
#include"ParticleManager.h"

class Camera;

class Emitter
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public://�ÓI�����o�֐�

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	/// <param name="name">�e�N�X�`����</param>
	/// <returns>�C���X�^���X</returns>
	static std::unique_ptr<Emitter> Create(const std::string name);

public://�����o�֐�

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Emitter() {};

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Emitter();

	/// <summary>
	/// �p�[�e�B�N���̔����n�_
	/// </summary>
	/// <param name="maxCount">����������̍ő�</param>
	/// <param name="maxFrame">��������</param>
	/// <param name="position">�������W</param>
	/// <param name="velocity">���x</param>
	/// <param name="accel">�����x</param>
	/// <param name="startScale">�����T�C�Y</param>
	/// <param name="endScale">�ŏI�T�C�Y</param>
	/// <param name="startColor">�����J���[</param>
	/// <param name="endColor">�ŏI�J���[</param>
	void InEmitter(int maxCount, int maxFrame, XMFLOAT3 position, XMFLOAT3 velocity,
		XMFLOAT3 accel, float startScale, float endScale, XMFLOAT4 startColor, XMFLOAT4 endColor);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �폜
	/// </summary>
	void ParticlAllDelete();

	/// <summary>
	/// �������Ă���p�[�e�B�N���̌�
	/// </summary>
	/// <returns></returns>
	int GetCount() { return count; }

private:
	//�p�[�e�B�N������
	int count = 0;
	//�p�[�e�B�N���N���X
	std::unique_ptr<ParticleManager> particleManager = nullptr;
};