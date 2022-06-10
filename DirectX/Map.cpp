#include "Map.h"
#include "Csv.h"

std::unique_ptr<Map> Map::Create()
{
	//インスタンスを生成
	Map* instance = new Map();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize();

	return std::unique_ptr<Map>(instance);
}

Map::~Map()
{
}

void Map::Initialize()
{
	blockModel[0] = Model::CreateFromOBJ("Square");
	blockModel[1] = Model::CreateFromOBJ("Square");
	blockModel[2] = Model::CreateFromOBJ("Square");
	blockModel[3] = Model::CreateFromOBJ("Square");

	//初回マップ
	LoadMap("Resources/Map/map0.csv");
}

void Map::LoadMap(std::string fileName)
{
	//csv読み込み
	std::vector<std::vector<int>> output;
	Csv::LoadFile(fileName, output);

	//高さ(y)の配列サイズ変更
	map.resize(output.size());

	const float BLOCK_SIZE = 1.0f;

	for (int y = 0; y < output.size(); y++)
	{
		for (int x = 0; x < output[0].size(); x++)
		{
			map[y].emplace_back();
			map[y][x].mapNumber = output[y][x];
			//0以外ならblockあり
			map[y][x].isBlock = 0 != map[y][x].mapNumber;

			int a = 0;

			//4はstart地点だがblockは1と同じ
			int blockNumber = 0;
			if (map[y][x].mapNumber >= 1&& map[y][x].mapNumber != 4)
			{
				blockNumber = map[y][x].mapNumber;
			}
			else if (map[y][x].mapNumber == 4)
			{
				blockNumber = 1;
			}

			if (blockNumber >= 1)
			{
				map[y][x].block = MapBlock::Create(blockModel[blockNumber].get());
				map[y][x].block->SetPosition({ -100 + x * BLOCK_SIZE,y * BLOCK_SIZE,0 });
			}
		}
	}
}

void Map::Update()
{
	for (int y = 0; y < map.size(); y++)
	{
		for (int x = 0; x < map[0].size(); x++)
		{
			if (!map[y][x].isBlock) { continue; }
			map[y][x].block->Update();
		}
	}
}

void Map::Draw()
{
	for (int y = 0; y < map.size(); y++)
	{
		for (int x = 0; x < map[0].size(); x++)
		{
			if (!map[y][x].isBlock) { continue; }
			map[y][x].block->Draw();
		}
	}
}
