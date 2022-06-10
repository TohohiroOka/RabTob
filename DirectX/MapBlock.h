#pragma once
#include "Object3d.h"

class MapBlock : public Object3d
{
public:

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<MapBlock> Create(Model* model = nullptr);

public:

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize() override;

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update() override;
};

