#pragma once
#include "DxLib.h"

#define BLOCK_MAX 6360
#define MAP_CHIP_X_NUM (212)
#define MAP_CHIP_Y_NUM (30)
#define MAP_CHIP_WIDTH (25.0f)
#define MAP_CHIP_HEIGHT (23.0f)

enum MapChipType
{
	MAP_CHIP_NONE,
	GRASS_BLOCK,
	NYANONE_SPAWN,
	NYANTWO_SPAWN,
	CAN_SPAWN,
	SOIL_BLOCK,
	NORMAL_BLOCK,
	MAP_DOOR,

	BLOCK_TYPE_MAX,
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

// マップの種類
enum MAP_TYPE
{
	MAP_STAGE1,        // 通常ステージ
	MAP_STAGE1_BOSS,   // BOSS部屋

	MAP_TYPE_MAX
};