#pragma once
#include <vector>
#include <string>

#include "Vector.h"
#include "Rect.h"

class MapGenerator
{
public:
	enum MapState
	{
		None = -1,
		Wall = 0,
		Room = 1,
		Pass = 2
	};

private:
	// 最小エリアサイズ
	const int MINIMUM_AREA_SIZE = 10;

	// マップサイズ
	int mapWidth;
	int mapHeight;

	// 最大部屋数
	int maxRoomNum;

	// マップデータ本体
	std::vector<std::vector<MapState>> map;

	// エリア
	std::vector<Rect> areaList;
	// 部屋
	std::vector<Rect> roomList;
	// 通路
	std::vector<Rect> passList;
	// 部屋が存在するエリアを記憶しておく
	std::vector<int> areaWhereRoomExists;

public:
	// コンストラクタ
	MapGenerator();

	// デストラクタ
	~MapGenerator();

	// マップ生成エントリ
	void Generate(int width, int height, int roomNum);

	std::vector<std::vector<MapState>> GetMapData();

	void ShowMap();

private:
	// マップ生成
	void _generate(int width, int height, int roomNum);

	// マップ初期化
	void _mapInitialize();

	// マップ削除
	void _deleteMap();

	// マップを特定の値で埋める
	void _fill(MapState fill);

	// エリア作成
	void _createArea();

	// エリア分割
	bool _splitArea(bool isVertical);

	// 部屋作成
	void _createRoom();

	// 通路作成
	void _createPass();

	// 部屋から通路を伸ばす
	void _extendPassFromRoom();
	// 伸ばした通路を繋ぐ
	void _connectPass();
	void __connectPass(int i, int k);

	// 作成した情報をマップに反映する
	void _reflectListIntoMap();

private:
	// デバッグ用ログ出力
	void DebugLog(std::string str);

	void ShowList();
};

