#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Camera/Camera.h"

//プレイヤー設定関連
#define PLAYER_DEFAULT_POS_X (100.0f)
#define PLAYER_DEFAULT_POS_Y (600.0f)
#define PLAYER_MOVE_SPEED (4.0f)

//ジャンプ力
#define PLAYER_JUMP_POWER (12.0f)
//重力
#define PLAYER_GRAVITY (0.4f)

PlayerData g_PlayerData = { 0 };
PlayerData g_PrevPlayerData = { 0 };

void InitPlayer()
{
	g_PlayerData.pos.x = PLAYER_DEFAULT_POS_X;
	g_PlayerData.pos.y = PLAYER_DEFAULT_POS_Y;
	g_PlayerData.move.x = 0.0f;
	g_PlayerData.move.y = 0.0f;
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
	//死んでたら何もしない
	if (!g_PlayerData.active)
	{
		return;
	}

	//X移動量は毎回リセット
	g_PlayerData.move.x = 0.0f;

	//Y移動量に重力を反映
	g_PlayerData.move.y += PLAYER_GRAVITY;

	//左右
	if (IsInputKey(KEY_RIGHT))
	{
		g_PlayerData.move.x = PLAYER_MOVE_SPEED;
	}
	if (IsInputKey(KEY_LEFT))
	{
		g_PlayerData.move.x = -PLAYER_MOVE_SPEED;
	}
	//ジャンプ
	if (IsInputKey(KEY_UP))
	{
		g_PlayerData.move.y = -PLAYER_JUMP_POWER;
	}
}

void UpdatePlayer()
{
	//死んで処理しない
	if (!g_PlayerData.active) return;

	g_PlayerData.pos.x += g_PlayerData.move.x;
	g_PlayerData.pos.y += g_PlayerData.move.y;
}

void DrawPlayer()
{
	
}

void FinPlayer()
{
	DeleteGraph(g_PlayerData.handle);
}

//PlayerData GetPlayer()
//{
//	
//}