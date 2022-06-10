#include "Texture.h"
#include <DirectXTex.h>
#include <string>

ID3D12Device* Texture::device = nullptr;

void Texture::StaticInitialize(ID3D12Device* device)
{
	// nullptr�`�F�b�N
	assert(!Texture::device);
	assert(device);
	Texture::device = device;

	DescriptorHeapManager::StaticInitialize(device);
}

std::unique_ptr<Texture> Texture::Create(const std::string fileName, ID3D12GraphicsCommandList* cmdList)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Texture* instance = new Texture();

	//Fbx�t�@�C���̓ǂݍ���
	if (cmdList == nullptr)
	{
		instance->LoadTexture(fileName);
	}
	else
	{
		instance->LoadTextureFromDDSFile(fileName, cmdList);
	}

	return std::unique_ptr<Texture>(instance);
}

Texture::~Texture()
{
	texBuffer.Reset();
	texConstBuffer.Reset();
}

void Texture::LoadTexture(const std::string fileName)
{
	HRESULT result;

	//�e�N�X�`���o�b�t�@�ԍ�
	static int texBuffNum = 0;

	////WIC�e�N�X�`���̃��[�h
	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImage{};

	//���j�R�[�h�ɕϊ�
	wchar_t wfilePath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0,
		fileName.c_str(), -1, wfilePath, _countof(wfilePath));

	result = LoadFromWICFile(
		wfilePath,
		DirectX::WIC_FLAGS_NONE,
		&metadata, scratchImage);
	assert(SUCCEEDED(result));

	const DirectX::Image* img = scratchImage.GetImage(0, 0, 0);

	//�e�N�X�`���o�b�t�@�̐���
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	//�e�N�X�`���o�b�t�@����
	result = device->CreateCommittedResource(//GPU���\�[�X�̐���
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//�e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&texBuffer));

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texBuffer->WriteToSubresource(
		0,
		nullptr,//�S�̈�փR�s�[
		img->pixels,//���f�[�^�A�h���X
		(UINT)img->rowPitch,//�P���C���T�C�Y
		(UINT)img->slicePitch//1���T�C�Y
	);

	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	descriptor = std::make_unique<DescriptorHeapManager>();
	descriptor->CreateSRV(texBuffer, srvDesc);
}

void Texture::LoadTextureFromDDSFile(const std::string fileName, ID3D12GraphicsCommandList* cmdList)
{
	HRESULT result;

	DirectX::TexMetadata metadata;
	DirectX::ScratchImage image;

	//���j�R�[�h�ɕϊ�
	wchar_t wfilePath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0,
		fileName.c_str(), -1, wfilePath, _countof(wfilePath));

	result = DirectX::LoadFromDDSFile(wfilePath, DirectX::DDS_FLAGS_NONE, &metadata, image);
	assert(SUCCEEDED(result));

	result = CreateTexture(device, metadata, &texBuffer);
	assert(SUCCEEDED(result));

	//���\�[�X�z��
	result = PrepareUpload(device, image.GetImages(), image.GetImageCount(), metadata, subresources);
	assert(SUCCEEDED(result));

	//�T�C�Y
	UINT totalBytes = (UINT)GetRequiredIntermediateSize(texBuffer.Get(), 0, UINT(subresources.size()));

	//�e�N�X�`���]���p�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(totalBytes),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texConstBuffer));
	assert(SUCCEEDED(result));

	UpdateSubresources(
		cmdList,
		texBuffer.Get(), texConstBuffer.Get(),
		0, 0, (UINT)subresources.size(), subresources.data());

	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		texBuffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	cmdList->ResourceBarrier(1, &barrier);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MipLevels = (UINT)metadata.mipLevels;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	descriptor = std::make_unique<DescriptorHeapManager>();
	descriptor->CreateSRV(texBuffer, srvDesc);
}
