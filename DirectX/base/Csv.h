#pragma once
#include <d3dx12.h>
#include <DirectXMath.h>

class Csv
{
protected: // �G�C���A�X
// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;

public:

	/// <summary>
	/// �}�b�v�̓ǂݍ���
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <param name="map">�}�b�v�z��</param>
	static void LoadFile(std::string fileName, std::vector<std::vector<int>>& map);

	/// <summary>
	/// �I�u�W�F�N�g���W�̔z��ԍ��̒l�̎擾
	/// </summary>
	/// <param name="map">�񎟌��z��</param>
	/// <param name="chipSize">����̑傫��</param>
	/// <param name="position">�I�u�W�F�N�g���W</param>
	/// <returns></returns>
	static int GetChipNum(std::vector<std::vector<int>> map, float chipSize, XMFLOAT2 position);
};