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
	// �ŏ��G���A�T�C�Y
	const int MINIMUM_AREA_SIZE = 10;

	// �}�b�v�T�C�Y
	int mapWidth;
	int mapHeight;

	// �ő啔����
	int maxRoomNum;

	// �}�b�v�f�[�^�{��
	std::vector<std::vector<MapState>> map;

	// �G���A
	std::vector<Rect> areaList;
	// ����
	std::vector<Rect> roomList;
	// �ʘH
	std::vector<Rect> passList;
	// ���������݂���G���A���L�����Ă���
	std::vector<int> areaWhereRoomExists;

public:
	// �R���X�g���N�^
	MapGenerator();

	// �f�X�g���N�^
	~MapGenerator();

	// �}�b�v�����G���g��
	void Generate(int width, int height, int roomNum);

	std::vector<std::vector<MapState>> GetMapData();

	void ShowMap();

private:
	// �}�b�v����
	void _generate(int width, int height, int roomNum);

	// �}�b�v������
	void _mapInitialize();

	// �}�b�v�폜
	void _deleteMap();

	// �}�b�v�����̒l�Ŗ��߂�
	void _fill(MapState fill);

	// �G���A�쐬
	void _createArea();

	// �G���A����
	bool _splitArea(bool isVertical);

	// �����쐬
	void _createRoom();

	// �ʘH�쐬
	void _createPass();

	// ��������ʘH��L�΂�
	void _extendPassFromRoom();
	// �L�΂����ʘH���q��
	void _connectPass();
	void __connectPass(int i, int k);

	// �쐬���������}�b�v�ɔ��f����
	void _reflectListIntoMap();

private:
	// �f�o�b�O�p���O�o��
	void DebugLog(std::string str);

	void ShowList();
};

