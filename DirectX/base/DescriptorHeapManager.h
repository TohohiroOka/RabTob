#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <array>

class DescriptorHeapManager
{
public:

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �������
	/// </summary>
	static void Finalize();

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	DescriptorHeapManager() {};

	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~DescriptorHeapManager();

	/// <summary>
	/// �V�F�[�_�[���\�[�X�r���[�̍쐬
	/// </summary>
	/// <param name="texBuffer">�e�N�X�`���o�b�t�@</param>
	/// <param name="srvDesc">�V�F�[�_�[���\�[�X�r���[�ݒ�</param>
	void CreateSRV(Microsoft::WRL::ComPtr<ID3D12Resource> texBuffer, D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc);

private:

	//�f�o�C�X
	static ID3D12Device* device;
	//�f�X�N���v�^�q�[�v
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
	//�f�X�N���v�^�̑傫��
	static const int DescriptorsSize = 512;
	//�f�X�N���v�^�e�[�u���̐���
	static std::array<bool, DescriptorsSize> TableManager;

public:

	int heapNumber = 0;
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpu;
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpu;
};