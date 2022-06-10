#pragma once
#include "MapBlock.h"
#include "Model.h"

class Map
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:

	struct STATE
	{
		int mapNumber = 0;//�}�b�v�`�b�v�ԍ�
		bool isBlock = false;//�u���b�N��ݒu���Ȃ��ꏊ�Ȃ�true�ɂ���
		std::unique_ptr<MapBlock> block = nullptr;//�u���b�N�̃C���X�^���X
	};

public:

	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static std::unique_ptr<Map> Create();

public:

	Map() {};
	~Map();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �}�b�v�ǂݍ���
	/// </summary>
	/// <param name="fileName">�}�b�v��</param>
	void LoadMap(std::string fileName);

	/// <summary>
	/// 
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private://�Œ�l

	//�}�b�v�u���b�N�̎��
	static const int mapBlockNum = 4;

private:

	//�}�b�v���
	std::vector<std::vector<STATE>> map;
	//�}�b�v�u���b�N�̃��f���z��
	std::array<std::unique_ptr<Model>, mapBlockNum> blockModel;
};

