#include "DxLib.h"
#include "Nyanone.h"
#include "Enemyparameter.h"
#include "../GameSetting/GameSetting.h"
#include "../Camera/Camera.h"
#include "../Animation/Animation.h"
#include "../Collision/Collision.h"
#include "../Map/MapManager.h"

#define NYANONE_MOVE_SPEED (0.5f)
#define NYANONE_GRAVITY (0.4f)

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
		one->isRight = false;
		one->param = nullptr;

		one->frameNum = 8;
		one->frameWidth = 37;
		one->frameHeight = 37;
		one->interval = 8;
		one->animTimer = 0;
	}
}

void LoadNyanone()
{
	g_NyanoneHandle = LoadGraph("Data/Effect/onewaik.png");

	for (int i = 0; i < ENEMY_NYANONE_MAX; i++)
	{
		g_NyanoneData[i].handle = g_NyanoneHandle;
	}
}

void StarNyanone()
{
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

		// 移動方向
		if (one->isRight)
		{
			// 左に移動
			one->move.x = -NYANONE_MOVE_SPEED;
		}
		else
		{
			// 右に移動
			one->move.x = NYANONE_MOVE_SPEED;
		}

		// 重力
		one->move.y += NYANONE_GRAVITY;
	}
}

void UpdateNyanone()
{
	EnemyNyanoneData* one = g_NyanoneData;

	for (int i = 0; i < ENEMY_NYANONE_MAX; i++, one++)
	{
		if (!one->active)
		{
			continue;
		}

		// 当たり判定用のデータ
		Body body = {};

		body.pos = one->pos;
		body.move = one->move;
		body.width = ENEMY_NYANONE_WIDTH;
		body.height = ENEMY_NYANONE_HEIGHT;

		// ブロックとの当たり判定
		MoveWithMapCollision(&body);

		// 壁に当たったら方向転換
		if (one->move.x != 0.0f && body.move.x == 0.0f)
		{
			one->isRight = !one->isRight;
		}

		// 当たり判定後の座標を戻す
		one->pos = body.pos;
		one->move = body.move;
	}
}

void DrawNyanone()
{
	CameraData camera = GetCamera();

	EnemyNyanoneData* one = g_NyanoneData;

	one->animTimer++;

	// 現在表示するコマ
	int animFrame = (one->animTimer / one->interval) % one->frameNum;

	for (int i = 0; i < ENEMY_NYANONE_MAX; i++, one++)
	{
		if (!one->active)
		{
			continue;
		}

		// 切り取る画像のX座標
		int srcX = animFrame * one->frameWidth;

		// 歩行アニメーション描画
		DrawRectGraph((int)(one->pos.x - camera.pos.x),(int)(one->pos.y - camera.pos.y + 11),
			srcX, 0, one->frameWidth, one->frameHeight, one->handle, TRUE, !one->isRight);
	}
}

void FinNyanone()
{
	DeleteGraph(g_NyanoneHandle);
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
		one->isRight = false;

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
void OneHitPlayer(int index)
{
	EnemyNyanoneData* one = &g_NyanoneData[index];

	if (!one->active) return;

	// 倒されたときのアニメーション
	PlayAnimationEffect(ENEMY_ONE_DEBT,one->pos.x,one->pos.y,5);

	// 敵を消す
	one->active = false;
}

void OneHitPlayerBullet(int index)
{
	EnemyNyanoneData* one = &g_NyanoneData[index];

	if (!one->active) return;

	// 倒されたときのアニメーション
	PlayAnimationEffect(ENEMY_ONE_DEBT, one->pos.x, one->pos.y, 5);

	// 敵を消す
	one->active = false;
}