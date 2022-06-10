#include "MapBlock.h"

std::unique_ptr<MapBlock> MapBlock::Create(Model* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	MapBlock* instance = new MapBlock();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	if (model) {
		instance->SetModel(model);
	}

	return std::unique_ptr<MapBlock>(instance);
}

bool MapBlock::Initialize()
{
	if (!Object3d::Initialize())
	{
		return false;
	}

	SetScale({ 0.5f,0.5f,0.5f });
	SetColor({ 1,1,1,1 });

	return true;
}

void MapBlock::Update()
{
	Object3d::Update();
}