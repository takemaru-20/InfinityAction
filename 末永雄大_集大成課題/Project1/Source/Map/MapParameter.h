#pragma once
#include "DxLib.h"

#define BLOCK_MAX 6360
#define MAP_CHIP_X_NUM (252)
#define MAP_CHIP_Y_NUM (30)
#define MAP_CHIP_WIDTH (25.0f)
#define MAP_CHIP_HEIGHT (23.0f)

enum MapChipType
{
	MAP_CHIP_NONE,		//0
	GRASS_BLOCK,		//1
	NYANONE_SPAWN,		//2
	NYANTWO_SPAWN,		//3
	CAN_SPAWN,			//4
	SOIL_BLOCK,			//5
	NORMAL_BLOCK,		//6
	MAP_DOOR,			//7
	ASND_BLOCK,			//8
	ASNDNORMAL_BLOCK,	//9
	ASNDTRIANGLE_BLOCK,	//10
	SANDUP_BLOCK,		//11

	BLOCK_TYPE_MAX
};

struct BlockData
{
	bool active;
	int handle;
	MapChipType type;
	VECTOR pos;
	float width;
	float height;
};

struct MapChipData
{
	int mapChip;
	BlockData* data;
};
