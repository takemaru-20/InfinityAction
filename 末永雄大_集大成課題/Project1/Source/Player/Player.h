#pragma once
#include "../Map/MapParameter.h"
#include "../Collision/CollisionParameter.h"

//プレイヤーサイズ
#define PLAYER_WIDTH	(40.0f)
#define PLAYER_HEIGHT	(29.0f)
#define PLAYER_RADIUS	(36.0f)

//プレイヤーデータ
struct PlayerData
{
	int handle;		//画像
	int life;
	bool active;	//生存フラグ
	Body body;		// 当たり判定ボディ
	bool inviibie;
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
PlayerData* GetPlayer();

//プレイヤー死亡処理
void PlayerDead();

void PlayerHitEnemy();
void PlayerHitEnemyBullet();
