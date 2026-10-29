#pragma once
#include <DxLib.h>

enum EnemyType
{
	ENEMY_TYPE_NYANNOE,
};

struct EnemyParameter
{
	float moveChangePosX;	//このX座標より左に行くと方向転換する
};

//Nyanone////////

#define ENEMY_NYANONE_MAX			(255)    //最大出現数
#define ENEMY_NYANONE_RADIUS		(20.0f)	 //半径	 
#define ENEMY_NYANONE_WIDTH			(36.0f)	 //横幅
#define ENEMY_NYANONE_HEIGHT		(36.0f)  //縦幅

struct EnemyNyanoneData
{
	int handle;
	int NYANONETimer;
	VECTOR pos;
	VECTOR move;
	bool active;
	bool isMoveChange;
	const EnemyParameter* param;
};