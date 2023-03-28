#include "MapGenerator.h"

#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <algorithm>
#include <random>

#include "Random.h"
#include "RougeUtils.h"

MapGenerator::MapGenerator()
{
}

MapGenerator::~MapGenerator()
{
}

void MapGenerator::Generate(int width, int height, int roomNum)
{
	_generate(width, height, roomNum);
}

std::vector<std::vector<MapGenerator::MapState>> MapGenerator::GetMapData()
{
	return this->map;
}

void MapGenerator::ShowMap()
{
	for (int y = 0; y < this->map.size(); y++)
	{
		for (int x = 0; x < this->map[y].size(); x++)
		{
			switch (this->map[y][x])
			{
			case MapState::None:
				std::cout << "■";
				break;
			case MapState::Wall:
				std::cout << "?";
				break;
			case MapState::Room:
				std::cout << "　";
				break;
			case MapState::Pass:
				std::cout << "　";
				break;
			}
		}
		std::cout << "\n";
	}
}

void MapGenerator::_generate(int width, int height, int roomNum)
{
	DebugLog("start map generator.");
	DebugLog("- - - initialize - - -");
	// 初期化
	this->mapWidth = width;
	this->mapHeight = height;
	this->maxRoomNum = roomNum;
	_mapInitialize();

	DebugLog(RougeUtils::CreateString("RESULT : {width = %d, height = %d, maxRoomNum = %d}", width, height, roomNum));

	DebugLog("- - - create - - -");
	// マップに必要な情報作成
	_createArea();
	_createRoom();
	_createPass();

	DebugLog("- - - reflect - - -");
	// 各情報の反映
	_reflectListIntoMap();

	DebugLog("map generator successful.");

	ShowList();
}

void MapGenerator::_mapInitialize()
{
	_deleteMap();

	for (int y = 0; y < this->mapHeight; y++)
	{
		this->map.push_back(std::vector<MapGenerator::MapState>());
		for (int x = 0; x < this->mapWidth; x++)
		{
			// とりあえずNoneで埋める
			this->map[y].push_back(MapState::None);
		}
	}
}

void MapGenerator::_deleteMap()
{
	for (int i = 0; i < map.size(); i++)
	{
		map[i].clear();
	}
	map.clear();

	this->areaList.clear();
	this->roomList.clear();
	this->passList.clear();

	this->areaWhereRoomExists.clear();
}

void MapGenerator::_fill(MapState fill)
{
	for (int y = 0; y < this->mapHeight; y++)
	{
		for (int x = 0; x < this->mapWidth; x++)
		{
			this->map[y][x] = fill;
		}
	}
}

void MapGenerator::_createArea()
{
	DebugLog("start create area.");
	// 最初にマップ全体をエリアとして設定
	this->areaList.push_back(Rect(0, 0, this->mapWidth - 1, this->mapHeight - 1));
	bool isDevided = true;
	int count = 0;
	while (isDevided)
	{
		DebugLog(RougeUtils::CreateString("split count:%d", count));
		// 縦→横　の順に分割していく
		isDevided = _splitArea(false);
		isDevided = _splitArea(true) || isDevided;

		// 最大部屋数に達したら終了
		if (this->areaList.size() >= this->maxRoomNum)
		{
			break;
		}
		count++;
	}
	DebugLog("end create area.");
}

bool MapGenerator::_splitArea(bool isVertical)
{
	bool isDevided = isVertical;
	Random rand = Random();
	std::vector<Rect> new_area_list = std::vector<Rect>();
	for (Rect& area : this->areaList)
	{
		// これ以上分割できない場合スキップする
		if (isVertical && area.GetHeight() < MINIMUM_AREA_SIZE * 2 + 1)
		{
			continue;
		}
		else if (!isVertical && area.GetWidth() < MINIMUM_AREA_SIZE * 2 + 1)
		{
			continue;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		// 40%の確率で分割しない
		// 区画が1つしかない場合は分割する
		if (areaList.size() > 1 && rand.Jadge(40))
		{
			continue;
		}

		// 分割方向
		int length = isVertical ? area.GetHeight() : area.GetWidth();
		int margin = length - MINIMUM_AREA_SIZE * 2;
		int base_index = isVertical ? area.start.y : area.start.x;
		int devide_index = base_index + MINIMUM_AREA_SIZE + rand.Range(1, margin) - 1;

		Rect new_area = Rect();

		// 分割した新しいエリアを作成し、分割元エリアのサイズを更新する
		if (isVertical)
		{
			new_area = Rect(area.start.x, devide_index + 1, area.end.x, area.end.y);
			area.end.y = devide_index - 1;
		}
		else
		{
			new_area = Rect(devide_index + 1, area.start.y, area.end.x, area.end.y);
			area.end.x = devide_index - 1;
		}
		// new_area_listに保存しておく
		new_area_list.push_back(new_area);
		isDevided = true;
	}

	// 分割したエリアを登録
	for (int i = 0; i < new_area_list.size(); i++)
	{
		this->areaList.push_back(new_area_list[i]);
	}

	return isDevided;
}

void MapGenerator::_createRoom()
{
	DebugLog("create room.");
	std::mt19937 mt;
	std::shuffle(this->areaList.begin(), this->areaList.end(), mt);

	Random rand = Random();

	for (int i = 0; i < this->areaList.size(); i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		if (this->roomList.size() > this->maxRoomNum / 2 && rand.Jadge(30))
		{
			continue;
		}
		Rect& area = areaList[i];

		int marginX = area.GetWidth() - MINIMUM_AREA_SIZE + 2;
		int marginY = area.GetHeight() - MINIMUM_AREA_SIZE + 2;

		int randomX = rand.Range(1, marginX);
		int randomY = rand.Range(1, marginY);

		int startX = area.start.x + randomX;
		int endX = area.end.x - rand.Range(0, (marginX - randomX)) - 1;
		int startY = area.start.y + randomY;
		int endY = area.end.y - rand.Range(0, (marginY - randomY)) - 1;

		Rect room = Rect(startX, startY, endX, endY);
		this->roomList.push_back(room);
		this->areaWhereRoomExists.push_back(i);
	}
}

void MapGenerator::_createPass()
{
	DebugLog("create pass.");
	// 部屋から通路を伸ばす
	_extendPassFromRoom();
	// 伸ばした通路を繋ぐ
	_connectPass();
}

void MapGenerator::_extendPassFromRoom()
{
	Random rand = Random();
	int count = 0;
	for (int i = 0; i < this->areaList.size(); i++)
	{
		// 部屋の存在しないエリアは無視する
		if (!RougeUtils::Contains(this->areaWhereRoomExists, i))
		{
			continue;
		}
		// this->areaList[i] == this->roomList[count]
		const Rect& room = this->roomList[count];
		// 部屋の中でのランダムな点を取る
		int randomX = rand.Range(room.start.x, room.end.x);
		int randomY = rand.Range(room.start.y, room.end.y);

		int startX = randomX;
		int startY = randomY;
		int endX = randomX;
		int endY = randomY;

		// 部屋の右側
		if (this->areaList[i].end.x < this->mapWidth - 1)
		{
			int target_x = areaList[i].end.x + 1;
			while (endX != target_x)
			{
				endX++;
			}
			Rect pass = Rect(startX, startY, endX, endY);
			this->passList.push_back(pass);

			startX = randomX;
			startY = randomY;
			endX = randomX;
			endY = randomY;
		}
		// 部屋の左側
		if (areaList[i].start.x > 0)
		{
			int target_x = areaList[i].start.x - 1;
			while (startX != target_x)
			{
				startX--;
			}
			Rect pass = Rect(startX, startY, endX, endY);
			this->passList.push_back(pass);

			startX = randomX;
			startY = randomY;
			endX = randomX;
			endY = randomY;
		}
		// 部屋の下側
		if (areaList[i].end.y < this->mapHeight - 1)
		{
			int target_y = areaList[i].end.y + 1;
			while (endY != target_y)
			{
				endY++;
			}
			Rect pass = Rect(startX, startY, endX, endY);
			this->passList.push_back(pass);

			startX = randomX;
			startY = randomY;
			endX = randomX;
			endY = randomY;
		}
		// 部屋の上側
		if (areaList[i].start.y > 0)
		{
			int target_y = areaList[i].start.y - 1;
			while (startY != target_y)
			{
				startY--;
			}
			Rect pass = Rect(startX, startY, endX, endY);
			this->passList.push_back(pass);
		}

		count++;
	}
}

void MapGenerator::_connectPass()
{
	int nowPassSize = this->passList.size();
	for (int i = 0; i < nowPassSize; i++)
	{
		for (int k = 0; k < nowPassSize; k++)
		{
			// 自分自身は無視
			if (i == k)
			{
				continue;
			}
			// 同じroomから伸びている通路は無視
			if (passList[i].start == passList[k].start ||
				passList[i].start == passList[k].end ||
				passList[i].end == passList[k].start ||
				passList[i].end == passList[k].end)
			{
				continue;
			}
			__connectPass(i, k);
		}
	}
}

void MapGenerator::__connectPass(int i, int k)
{
	const Rect& v1 = passList[i];
	const Rect& v2 = passList[k];
	Rect pass;
	{
		if (v1.start.x == v2.start.x)
		{
			if (v1.start.y < v2.start.y)
			{
				pass = Rect(v1.start, v2.start);
				this->passList.push_back(pass);
				return;
			}
			if (v1.start.y > v2.start.y)
			{
				pass = Rect(v2.start, v1.start);
				this->passList.push_back(pass);
				return;
			}
		}
		if (v1.start.x == v2.end.x)
		{
			if (v1.start.y < v2.end.y)
			{
				pass = Rect(v1.start, v2.end);
				this->passList.push_back(pass);
				return;
			}
			if (v1.start.y > v2.end.y)
			{
				pass = Rect(v2.end, v1.start);
				this->passList.push_back(pass);
				return;
			}
		}
		if (v1.end.x == v2.end.x)
		{
			if (v1.end.y < v2.end.y)
			{
				pass = Rect(v1.end, v2.end);
				this->passList.push_back(pass);
				return;
			}
			if (v1.end.y > v2.end.y)
			{
				pass = Rect(v2.end, v1.end);
				this->passList.push_back(pass);
				return;
			}
		}
	}
	{
		if (v1.start.y == v2.start.y)
		{
			if (v1.start.x < v2.start.x)
			{
				pass = Rect(v1.start, v2.start);
				this->passList.push_back(pass);
				return;
			}
			if (v1.start.x > v2.start.x)
			{
				pass = Rect(v2.start, v1.start);
				this->passList.push_back(pass);
				return;
			}
		}
		if (v1.start.y == v2.end.y)
		{
			if (v1.start.x < v2.end.x)
			{
				pass = Rect(v1.start, v2.end);
				this->passList.push_back(pass);
				return;
			}
			if (v1.start.x > v2.end.x)
			{
				pass = Rect(v2.end, v1.start);
				this->passList.push_back(pass);
				return;
			}
		}
		if (v1.end.y == v2.end.y)
		{
			if (v1.end.x < v2.end.x)
			{
				pass = Rect(v1.end, v2.end);
				this->passList.push_back(pass);
				return;
			}
			if (v1.end.x > v2.end.x)
			{
				pass = Rect(v2.end, v1.end);
				this->passList.push_back(pass);
				return;
			}
		}
	}
}

void MapGenerator::_reflectListIntoMap()
{
	for (const Rect& pass : this->passList)
	{
		for (int y = pass.start.y; y <= pass.end.y; y++)
		{
			for (int x = pass.start.x; x <= pass.end.x; x++)
			{
				this->map[y][x] = MapState::Pass;
			}
		}
	}
	for (const Rect& room : this->roomList)
	{
		for (int y = room.start.y; y <= room.end.y; y++)
		{
			for (int x = room.start.x; x <= room.end.x; x++)
			{
				this->map[y][x] = MapState::Room;
			}
		}
	}
}

void MapGenerator::DebugLog(std::string str)
{
	std::cout << "[ DEBUG ]: " << str << std::endl;
}

void MapGenerator::ShowList()
{
	for (const Rect& area : this->areaList)
	{
		DebugLog(RougeUtils::CreateString("area start:(%d, %d), end:(%d, %d)", area.start.x, area.start.y, area.end.x, area.end.y));
	}
	for (const Rect& room : this->roomList)
	{
		DebugLog(RougeUtils::CreateString("room start:(%d, %d), end:(%d, %d)", room.start.x, room.start.y, room.end.x, room.end.y));
	}
	for (const Rect& pass : this->passList)
	{
		DebugLog(RougeUtils::CreateString("pass start:(%d, %d), end:(%d, %d)", pass.start.x, pass.start.y, pass.end.x, pass.end.y));
	}
}
