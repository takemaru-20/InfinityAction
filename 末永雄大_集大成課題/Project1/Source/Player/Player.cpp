#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Camera/Camera.h"
#include "../Map/MapManager.h"
#include "../Collision/Collision.h"

//プレイヤー設定関連
#define PLAYER_DEFAULT_POS_X (150.0f)
#define PLAYER_DEFAULT_POS_Y (300.0f)
#define PLAYER_MOVE_SPEED (1.0f)

//ジャンプ力
#define PLAYER_JUMP_POWER (12.0f)

//重力
#define PLAYER_GRAVITY (0.4f)

PlayerData g_PlayerData = { 0 };

void InitPlayer()
{
	g_PlayerData.body.pos.x = PLAYER_DEFAULT_POS_X;
	g_PlayerData.body.pos.y = PLAYER_DEFAULT_POS_Y;

	g_PlayerData.body.move.x = 0.0f;
	g_PlayerData.body.move.y = 0.0f;

	g_PlayerData.body.width = PLAYER_WIDTH;
	g_PlayerData.body.height = PLAYER_HEIGHT;
}

void LoadPlayer()
{
	g_PlayerData.handle = LoadGraph("Data/Player/Player_Normal.png");
}

void StartPlayer()
{
	//生存フラグ
	g_PlayerData.active = true;
}

void StepPlayer()
{
	if (!g_PlayerData.active)
	{
		return;
	}

	// X移動量は毎回リセットする
	g_PlayerData.body.move.x = 0.0f;

	// Y移動量に重力を反映
	g_PlayerData.body.move.y += PLAYER_GRAVITY;

	//左右移動
	if (IsInputKey(KEY_RIGHT))
	{
		g_PlayerData.body.move.x = PLAYER_MOVE_SPEED;
	}

	if (IsInputKey(KEY_LEFT))
	{
		g_PlayerData.body.move.x = -PLAYER_MOVE_SPEED;
	}

	//ジャンプ
	if (IsTriggerKey(KEY_UP))
	{
		g_PlayerData.body.move.y = -PLAYER_JUMP_POWER;
	}
}

void UpdatePlayer()
{
	//死んでたら処理しない
	if (!g_PlayerData.active) return;

	// 当たり判定付きで移動
	MoveWithMapCollision(&g_PlayerData.body);
}

void DrawPlayer()
{
	//死んでたら処理しない
	if (!g_PlayerData.active) return;

	//カメラ取得
	CameraData camera = GetCamera();

	DrawGraph(
		(int)(g_PlayerData.body.pos.x - camera.pos.x),
		(int)(g_PlayerData.body.pos.y - camera.pos.y),
		g_PlayerData.handle,
		TRUE
	);
}

void FinPlayer()
{
	DeleteGraph(g_PlayerData.handle);
}

PlayerData* GetPlayer()
{
	return &g_PlayerData;
}

void PlayerDead()
{
	//プレイヤーを死亡状態にする
	g_PlayerData.active = false;

	//移動停止
	g_PlayerData.body.move.x = 0.0f;
	g_PlayerData.body.move.y = 0.0f;
}

void PlayerHitEnemy()
{
	PlayerDead();
}

void PlayerHitEnemyBullet()
{
	PlayerDead();
}
