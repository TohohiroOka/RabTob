#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>

#include "GraphicsPipelineManager.h"
#include "Texture.h"

class Sprite
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
	struct VertexPosUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT2 uv;  // uv���W
	};

	/// <summary>
	/// �萔�o�b�t�@�p�f�[�^�\����
	/// </summary>
	struct ConstBufferData
	{
		XMFLOAT4 color;	// �F (RGBA)
		XMMATRIX mat;	// �R�c�ϊ��s��
	};

	struct Information
	{
		bool isDelete = false; //�V�[���J�ڂō폜���s����
		std::unique_ptr<Texture> instance = nullptr;
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
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="keepName">�ۑ���</param>
	/// <param name="filename">�摜�t�@�C����</param>
	/// <param name="isDelete">�V�[���J�ڂō폜���s����</param>
	static void LoadTexture(const std::string keepName, const std::string filename, const bool isDelete = true);

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
	/// <param name="name">�e�N�X�`���ۑ���</param>
	/// <returns>�C���X�^���X</returns>
	static std::unique_ptr<Sprite> Create(const std::string name);

	/// <summary>
	/// �V�[�����Ƃ̉������
	/// </summary>
	static void SceneFinalize();

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
	//�e�N�X�`�����
	static std::map<std::string, Information> texture;
	// �ˉe�s��
	static XMMATRIX matProjection;

public: // �����o�֐�

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Sprite() {};

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Sprite();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="name">�e�N�X�`���ۑ���</param>
	/// <param name="anchorpoint">�A���J�[�|�C���g</param>
	/// <param name="isFlipX">���E���]���邩</param>
	/// <param name="isFlipY">�㉺���]���邩</param>
	/// <returns>����</returns>
	bool Initialize(const std::string name, const XMFLOAT2 anchorpoint, bool isFlipX = false, bool isFlipY = false);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

protected: // �����o�ϐ�

	//�e�N�X�`����
	std::string name;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// Z�����̉�]�p
	float rotation = 0.0f;
	// ���W
	XMFLOAT2 position{ 0, 0 };
	// �X�v���C�g���A����
	XMFLOAT2 size = { 100.0f, 100.0f };
	// �A���J�[�|�C���g
	XMFLOAT2 anchorpoint = { 0, 0 };
	// ���[���h�s��
	XMMATRIX matWorld{};
	// �F
	XMFLOAT4 color = { 1, 1, 1, 1 };
	// ���E���]
	bool isFlipX = false;
	// �㉺���]
	bool isFlipY = false;
	// �e�N�X�`���n�_
	XMFLOAT2 texLeftTop = { 0, 0 };
	// �e�N�X�`�����A����
	XMFLOAT2 texSize = { 500.0f, 500.0f };

protected: // �����o�֐�

	/// <summary>
	/// ���_�f�[�^�]��
	/// </summary>
	void TransferVertices();

public:

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	const XMFLOAT2& GetPosition() { return position; }

	/// <summary>
	/// ��]�p�̎擾
	/// </summary>
	/// <returns>��]�p</returns>
	const float GetRotation() { return rotation; }

	/// <summary>
	/// �傫���̎擾
	/// </summary>
	/// <returns>�傫��</returns>
	const XMFLOAT2& GetSize() { return size; }

	/// <summary>
	/// �����̎擾
	/// </summary>
	/// <returns>����</returns>
	const XMFLOAT2& GetTexSize() { return texSize; }

	/// <summary>
	/// �I�_�̎擾
	/// </summary>
	/// <returns>�I�_</returns>
	const XMFLOAT2& GetTexLeftTop() { return texLeftTop; }

	/// <summary>
	/// �F�̎擾
	/// </summary>
	/// <returns>�F</returns>
	const XMFLOAT4& GetColor() { return color; }

	/// <summary>
	/// �A���J�[�|�C���g�̎擾
	/// </summary>
	/// <returns>�A���J�[�|�C���g</returns>
	const XMFLOAT2& GetAnchorpoint() { return anchorpoint; }

	/// <summary>
	/// ���E���]�t���O�̎擾
	/// </summary>
	/// <returns>���E���]�t���O</returns>
	const bool GetIsFlipX() { return isFlipX; }

	/// <summary>
	/// �㉺���]�t���O�̎擾
	/// </summary>
	/// <returns>���E���]�t���O</returns>
	const bool GetIsFlipY() { return isFlipY; }

	/// <summary>
	/// �e�N�X�`���̃Z�b�g
	/// </summary>
	void SetTexNumber(std::string name) { this->name = name; };

	/// <summary>
	/// ���W�̓���
	/// </summary>
	void SetPosition(const XMFLOAT2& position) { this->position = position; }

	/// <summary>
	/// ��]�p�̓���
	/// </summary>
	void SetRotation(float rotation) { this->rotation = rotation; }

	/// <summary>
	/// �傫���̓���
	/// </summary>
	void SetSize(const XMFLOAT2& size) { this->size = size; }

	/// <summary>
	/// �����̓���
	/// </summary>
	void SetTexSize(const XMFLOAT2& texSize) { this->texSize = texSize; }

	/// <summary>
	/// �I�_�̓���
	/// </summary>
	void SetTexLeftTop(const XMFLOAT2& texLeftTop) { this->texLeftTop = texLeftTop; }

	/// <summary>
	/// �F�̓���
	/// </summary>
	void SetColor(const XMFLOAT4& color) { this->color = color; }

	/// <summary>
	/// �A���J�[�|�C���g�̓���
	/// </summary>
	void SetAnchorpoint(const XMFLOAT2& anchorpoint) { this->anchorpoint = anchorpoint; }

	/// <summary>
	/// ���E���]�t���O�̓���
	/// </summary>
	void SetIsFlipX(bool isFlipX) { this->isFlipX = isFlipX; };

	/// <summary>
	/// �㉺���]�t���O�̓���
	/// </summary>
	void SetIsFlipY(bool isFlipY) { this->isFlipY = isFlipY; };
};