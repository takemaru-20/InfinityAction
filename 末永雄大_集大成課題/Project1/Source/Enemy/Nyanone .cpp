#include "DxLib.h"
#include "Nyanone.h"
#include "Enemyparameter.h"
#include "../GameSetting/GameSetting.h"
#include "../Camera/Camera.h"
#include "../Animation/Animation.h"

#define NYANONE_MOVE_SPEED (0.5f)

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

		// 左に移動
		one->move.x = -NYANONE_MOVE_SPEED;
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

		one->pos.x +=one->move.x;
	}
}

void DrawNyanone()
{
	CameraData camera = GetCamera();

	EnemyNyanoneData* one = g_NyanoneData;

	// 歩行アニメーションの設定
	const int frameNum = 4;       // コマ数
	const float frameWidth = 37.5;    // 1コマの横幅
	const float frameHeight = 37.5;   // 1コマの縦幅
	const int interval = 8;       // 切り替え間隔

	// アニメーション用タイマー
	static int animTimer = 0;

	animTimer++;

	// 現在表示するコマ
	int animFrame = (animTimer / interval) % frameNum;

	for (int i = 0; i < ENEMY_NYANONE_MAX; i++, one++)
	{
		if (!one->active)
		{
			continue;
		}

		// 切り取る画像のX座標
		int srcX = animFrame * frameWidth;

		// 歩行アニメーション描画
		DrawRectGraph((int)(one->pos.x - camera.pos.x),(int)(one->pos.y - camera.pos.y),
			srcX, 0, frameWidth, frameHeight, one->handle, TRUE);
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

		one->pos.x = x;
		one->pos.y = y -5.0f;
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