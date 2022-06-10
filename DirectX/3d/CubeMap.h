#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include "GraphicsPipelineManager.h"
#include "Texture.h"

class Camera;

class CubeMap
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	//���_�f�[�^3D
	struct Vertex {
		XMFLOAT3 pos;
		XMFLOAT3 uv;
	};

	//�萔�o�b�t�@�̍\����
	struct ConstBufferData {
		XMMATRIX viewproj; // �r���[�v���W�F�N�V�����s��
		XMMATRIX world; // ���[���h�s��
		XMFLOAT3 cameraPos; // �J�������W�i���[���h���W�j
	};

private:

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	static void CreateGraphicsPipeline();

public://�����o�֐�

	/// <summary>
	/// �ÓI������
	/// </summary>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// directXCommon����
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static std::unique_ptr<CubeMap> Create(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �J�����̃Z�b�g
	/// </summary>
	/// <param name="camera">�J����</param>
	static void SetCamera(Camera* camera) {
		CubeMap::camera = camera;
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
	/// �������
	/// </summary>
	static void Finalize();

private://�����o�֐�

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

public:

	CubeMap() {};
	~CubeMap();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �L���[�u�}�b�v�̓]��
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	/// <param name="RootParameterIndex"></param>
	void TransferTextureBubber(ID3D12GraphicsCommandList* cmdList,UINT RootParameterIndex);

	/// <summary>
	/// �L���[�u�}�b�v�̃Z�b�g
	/// </summary>
	/// <returns></returns>
	Texture* SetTexture() { return texture.get(); }

private:

	//�f�o�C�X
	static ID3D12Device* device;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//�p�C�v���C��
	static std::unique_ptr<GraphicsPipelineManager> pipeline;
	//�J����
	static Camera* camera;
	//�C���f�b�N�X��
	static const int indexNum = 36;

private:

	//�e�N�X�`�����
	std::unique_ptr<Texture> texture = nullptr;
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//�L���[�u�}�b�v�o�b�t�@
	ComPtr<ID3D12Resource> texConstBuffer;
	//���\�[�X�z��
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//���W
	XMFLOAT3 position = { 0,400,0 };
	//�傫��
	XMFLOAT3 scale = { 500,500,500 };
	//��]
	XMFLOAT3 rotation = {};

};