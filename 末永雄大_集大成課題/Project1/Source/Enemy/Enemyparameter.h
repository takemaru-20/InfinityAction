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

	int frameNum;
	float frameWidth;
	float frameHeight;
	int interval;
	int animTimer;
};

//Nyantwo////////

#define ENEMY_NYANTWO_MAX			(255)    //最大出現数
#define ENEMY_NYANTWO_RADIUS		(20.0f)	 //半径	 
#define ENEMY_NYANTWO_WIDTH			(36.0f)	 //横幅
#define ENEMY_NYANTWO_HEIGHT		(36.0f)  //縦幅

struct EnemyNyantwoData
{
	int handle;
	VECTOR pos;
	VECTOR move;
	bool active;
	bool isMoveChange;
	const EnemyParameter* param;

	int JumpTimer;       // ジャンプまでの時間
	int Jumpdistance;    // ジャンプの強さ
	float JumpSpeed;     // 現在のジャンプ速度
	float GroundY;       // 地面のY座標

	int frameNum;
	float frameWidth;
	float frameHeight;
	int interval;
	int animTimer;
};


//Can////////

#define ENEMY_CAN_MAX			(255)    //最大出現数
#define ENEMY_CAN_RADIUS		(20.0f)	 //半径	 
#define ENEMY_CAN_WIDTH			(36.0f)	 //横幅
#define ENEMY_CAN_HEIGHT		(36.0f)  //縦幅

struct EnemyCanData
{
	int handle;
	VECTOR pos;
	VECTOR move;
	bool active;
	bool isMoveChange;
	const EnemyParameter* param;

	int frameNum;
	float frameWidth;
	float frameHeight;
	int interval;
	int animTimer;
};