#include "MapBlock.h"

std::unique_ptr<MapBlock> MapBlock::Create(Model* model)
{
	// 3Dオブジェクトのインスタンスを生成
	MapBlock* instance = new MapBlock();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
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