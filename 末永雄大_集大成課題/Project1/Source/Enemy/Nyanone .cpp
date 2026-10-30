#include "DxLib.h"
#include "Nyanone.h"
#include "Enemyparameter.h"
#include "../GameSetting/GameSetting.h"
#include "../Camera/Camera.h"

#define NYANONE_MOVE_SPEED (1.0f)
#define NYANONE_BREAK_X	(1.5f)
#define NYANONE_BREAK_Y	(0.30f)

// 変数
EnemyNyanoneData g_NyanoneData[ENEMY_NYANONE_MAX] = { 0 };

// 画像ハンドル
int g_NyanoneHandle = 0;

void InitNyanone()
{
	EnemyNyanoneData* one = g_NyanoneData;

	for (int i = 0; i < ENEMY_NYANONE_MAX; i++, one++)
	{
		one->active = false;

		one->pos = VGet(0.0f,0.0f,0.0f);

		one->move = VGet(0.0f,0.0f,0.0f);

		one->handle = 0;
		one->isMoveChange = false;
		one->param = nullptr;
	}
}

void LoadNyanone()
{
	g_NyanoneHandle = LoadGraph("Data/Enemy/Nyanone.png");

	for (int i = 0; i < ENEMY_NYANONE_MAX; i++)
	{
		g_NyanoneData[i].handle = g_NyanoneHandle;
	}
}

void StarNyanone()
{
}

void SpawnNyanone(float x, float y)
{
	for (int i = 0; i < ENEMY_NYANONE_MAX; i++)
	{
		EnemyNyanoneData* one = &g_NyanoneData[i];

		// 既に使われている
		if (one->active)
		{
			continue;
		}

		one->active = true;

		one->pos.x = x;
		one->pos.y = y;
		one->pos.z = 0.0f;

		one->move.x = 0.0f;
		one->move.y = 0.0f;
		one->move.z = 0.0f;

		one->handle = g_NyanoneHandle;

		return;
	}
}

EnemyNyanoneData* GetNyanone()
{
	return g_NyanoneData;
}



void StepNyanone()
{
	EnemyNyanoneData* one = g_NyanoneData;

	for (int i = 0; i < ENEMY_NYANONE_MAX; i++, one++)
	{
		// 死んでいたら何もしない
		if (!one->active)
		{
			continue;
		}

		// 左に移動
		one->move.x = -NYANONE_MOVE_SPEED;
	}
}

void UpdateNyanone()
{
	EnemyNyanoneData* one = g_NyanoneData;

	for (int i = 0;
		i < ENEMY_NYANONE_MAX;
		i++, one++)
	{
		if (!one->active)
		{
			continue;
		}

		one->pos.x +=
			one->move.x;

		one->pos.y +=
			one->move.y;
	}
}

void DrawNyanone()
{
	//カメラ取得
	CameraData camera = GetCamera();

	EnemyNyanoneData* one = g_NyanoneData;

	for (int i = 0; i < ENEMY_NYANONE_MAX; i++, one++)
	{
		if (!one->active) continue;

		DrawGraph((int)(one->pos.x - camera.pos.x),(int)(one->pos.y - camera.pos.y),g_NyanoneHandle,TRUE);
	}
}

void FinNyanone()
{
	DeleteGraph(g_NyanoneHandle);
}

void oneHitPlayer(int index)
{
	g_NyanoneData[index].active = false;
}

void oneHitPlayerBullet(int index)
{
	g_NyanoneData[index].active = false;
}