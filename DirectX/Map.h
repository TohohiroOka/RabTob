#pragma once
#include "MapBlock.h"
#include "Model.h"

class Map
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:

	struct STATE
	{
		int mapNumber = 0;//マップチップ番号
		bool isBlock = false;//ブロックを設置しない場所ならtrueにする
		std::unique_ptr<MapBlock> block = nullptr;//ブロックのインスタンス
	};

public:

	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static std::unique_ptr<Map> Create();

public:

	Map() {};
	~Map();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// マップ読み込み
	/// </summary>
	/// <param name="fileName">マップ名</param>
	void LoadMap(std::string fileName);

	/// <summary>
	/// 
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private://固定値

	//マップブロックの種類
	static const int mapBlockNum = 4;

private:

	//マップ情報
	std::vector<std::vector<STATE>> map;
	//マップブロックのモデル配列
	std::array<std::unique_ptr<Model>, mapBlockNum> blockModel;
};

