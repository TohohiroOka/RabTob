#pragma once
#include "Object3d.h"

class MapBlock : public Object3d
{
public:

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<MapBlock> Create(Model* model = nullptr);

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;
};

