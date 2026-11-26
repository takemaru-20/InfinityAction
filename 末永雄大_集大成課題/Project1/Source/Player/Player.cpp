#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Camera/Camera.h"
#include "../Map/MapManager.h"
#include "../Collision/Collision.h"
#include "../Sound/SoundManager.h"

//プレイヤー設定関連
#define PLAYER_DEFAULT_POS_X (150.0f)
#define PLAYER_DEFAULT_POS_Y (300.0f)
#define PLAYER_MOVE_SPEED (2.0f)
//#define PLAYER_MOVE_SPEED (10.0f)		//Debugging

//ジャンプ力
#define PLAYER_JUMP_POWER (8.0f)
// 流砂の中でのジャンプ力
#define PLAYER_QUICKSAND_JUMP_POWER (2.5f)

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
	// プレイヤー画像を読み込む
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
	
	// ジャンプ
	if (IsTriggerKey(KEY_UP))
	{
			// 通常のジャンプ力
			float jumpPower = PLAYER_JUMP_POWER;

			// 流砂の中だけジャンプ力を下げる
			if (IsPlayerInQuicksand())
			{
				jumpPower = PLAYER_QUICKSAND_JUMP_POWER;
			}

			// ジャンプ
			g_PlayerData.body.move.y = -jumpPower;
			//ジャンプ音SE
			PlaySE(SE_PLAYER_JUMP);

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
		g_PlayerData.handle,TRUE);
}



void FinPlayer()
{
	// プレイヤー画像を解放
	DeleteGraph(g_PlayerData.handle);
}


PlayerData* GetPlayer()
{
	return &g_PlayerData;
}


void PlayerDead()
{
	// すでに死亡している場合は何もしない
	if (!g_PlayerData.active)
	{
		return;
	}

	// 死亡効果音を再生
	PlaySE(SE_PLAYER_DEAD);

	// プレイヤーを死亡状態にする
	g_PlayerData.active = false;

	// 移動停止
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
