#include "DxLib.h"
#include "Canthrowing.h"
#include "Enemyparameter.h"
#include "../GameSetting/GameSetting.h"
#include "../Camera/Camera.h"
#include "../Animation/Animation.h"

// 変数
EnemyCanData g_CanData[ENEMY_CAN_MAX] = { 0 };

// 画像ハンドル
int g_CanHandle = 0;

void InitCan()
{
	EnemyCanData* can = g_CanData;

	for (int i = 0; i < ENEMY_CAN_MAX; i++, can++)
	{
		can->active = false;
		can->pos = VGet(0.0f, 0.0f, 0.0f);
		can->move = VGet(0.0f, 0.0f, 0.0f);
		can->handle = 0;
		can->isMoveChange = false;
		can->param = nullptr;

		can->frameNum = 8;
		can->frameWidth = 37;
		can->frameHeight = 37;
		can->interval = 8;
		can->animTimer = 0;
	}
}

void LoadCan()
{
	g_CanHandle = LoadGraph("Data/Effect/onewaik.png");

	for (int i = 0; i < ENEMY_CAN_MAX; i++)
	{
		g_CanData[i].handle = g_CanHandle;
	}
}

void StarCan()
{
}

void StepCan()
{
	EnemyCanData* can = g_CanData;

	for (int i = 0; i < ENEMY_CAN_MAX; i++, can++)
	{
		// 死んでいたら何もしない
		if (!can->active)
		{
			continue;
		}
	}
}

void UpdateCan()
{
	EnemyCanData* can = g_CanData;

	for (int i = 0; i < ENEMY_CAN_MAX; i++, can++)
	{
		if (!can->active)
		{
			continue;
		}
	}
}

void DrawCan()
{
	CameraData camera = GetCamera();

	EnemyCanData* can = g_CanData;

	can->animTimer++;

	// 現在表示するコマ
	int animFrame = (can->animTimer / can->interval) % can->frameNum;

	for (int i = 0; i < ENEMY_CAN_MAX; i++, can++)
	{
		if (!can->active)
		{
			continue;
		}

		// 切り取る画像のX座標
		int srcX = animFrame * can->frameWidth;

		// 歩行アニメーション描画
		DrawRectGraph((int)(can->pos.x - camera.pos.x), (int)(can->pos.y - camera.pos.y),
			srcX, 0, can->frameWidth, can->frameHeight, can->handle, TRUE);
	}
}

void FinCan()
{
	DeleteGraph(g_CanHandle);
}

void SpawnCan(float x, float y)
{
	for (int i = 0; i < ENEMY_CAN_MAX; i++)
	{
		EnemyCanData* can = &g_CanData[i];

		// 既に使われている
		if (can->active)
		{
			continue;
		}

		can->active = true;

		can->pos.x = x;
		can->pos.y = y - 5.0f;
		can->pos.z = 0.0f;

		can->move.x = 0.0f;
		can->move.y = 0.0f;
		can->move.z = 0.0f;

		can->handle = g_CanHandle;

		return;
	}
}

EnemyCanData* GetCan()
{
	return g_CanData;
}
void CanHitPlayer(int index)
{
	EnemyCanData* can = &g_CanData[index];

	if (!can->active) return;

	// 倒されたときのアニメーション
	PlayAnimationEffect(ENEMY_ONE_DEBT, can->pos.x, can->pos.y, 5);

	// 敵を消す
	can->active = false;
}

void CanHitPlayerBullet(int index)
{
	EnemyCanData* can = &g_CanData[index];

	if (!can->active) return;

	// 倒されたときのアニメーション
	PlayAnimationEffect(ENEMY_ONE_DEBT, can->pos.x, can->pos.y, 5);

	// 敵を消す
	can->active = false;
}