#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <map>

class ShaderManager
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	ShaderManager() {};
	~ShaderManager() {};

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<ShaderManager> Create();

	/// <summary>
	/// �V�F�[�_�[�̓ǂݍ���
	/// </summary>
	void LoadShader();

	/// <summary>
	/// �������
	/// </summary>
	void Finalize();

public:

	//���_�V�F�[�_�[�̃I�u�W�F�N�g
	std::map<std::string, ComPtr<ID3DBlob>> shaderObjectVS;
	//�s�N�Z���V�F�[�_�[�̃I�u�W�F�N�g
	std::map<std::string, ComPtr<ID3DBlob>> shaderObjectPS;
	//�W�I���g���V�F�[�_�V�F�[�_�[�̃I�u�W�F�N�g
	std::map<std::string, ComPtr<ID3DBlob>> shaderObjectGS;
	//�R���s���[�g�V�F�[�_�[�̃I�u�W�F�N�g
	std::map<std::string, ComPtr<ID3DBlob>> shaderObjectCS;
};

