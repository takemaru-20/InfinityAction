#pragma once
#include "../Map/MapParameter.h"

//プレイヤーサイズ
#define PLAYER_WIDTH	(72.0f)
#define PLAYER_HEIGHT	(72.0f)
#define PLAYER_RADIUS	(36.0f)

//プレイヤーデータ
struct PlayerData
{
	int handle;
	bool active;	//生存フラグ
	VECTOR pos;
	VECTOR move;
};

//関数のプロトタイプ宣言
void InitPlayer();
void LoadPlayer();
void StartPlayer();
void StepPlayer();
void UpdatePlayer();
void DrawPlayer();
void FinPlayer();

//取得関数
PlayerData GetPlayer();

//当たり判定
void PlayerHitNormalBlockX(MapChipData mapChipData);
void PlayerHitNormalBlockY(MapChipData mapChipData);