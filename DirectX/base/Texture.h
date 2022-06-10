#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>

#include "DescriptorHeapManager.h"

class Texture
{
public:

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <param name="cmdList">dds�t�@�C������cmdList�������Ă���</param>
	static std::unique_ptr<Texture> Create(const std::string fileName, ID3D12GraphicsCommandList* cmdList = nullptr);

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Texture() {};

	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~Texture();

	/// <summary>
	/// �e�N�X�`���̓ǂݍ���
	/// </summary>
	/// <param name="fileName">�摜�t�@�C���̖��O</param
	void LoadTexture(const std::string fileName);

	/// <summary>
	/// dds�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="fileName">�摜�t�@�C���̖��O</param>
	void LoadTextureFromDDSFile(const std::string fileName, ID3D12GraphicsCommandList* cmdList);

private:

	//�f�o�C�X
	static ID3D12Device* device;

public:
	//�e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuffer = nullptr;
	//�e�N�X�`���̃f�X�N���v�^
	std::unique_ptr<DescriptorHeapManager> descriptor = nullptr;
	//dds�݂̂Ɏg�p����
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	Microsoft::WRL::ComPtr<ID3D12Resource> texConstBuffer = nullptr;

};

