#pragma once
#include "DxLib.h"

struct Body
{
	bool isAir;
	VECTOR pos;
	VECTOR move;
	float width;
	float height;
	float radius;
	float hitBlock;
	float reflactPower;
};