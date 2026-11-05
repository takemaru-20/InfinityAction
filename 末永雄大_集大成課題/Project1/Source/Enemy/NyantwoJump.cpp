#include "DxLib.h"
#include "NyantwoJump.h"
#include "Enemyparameter.h"
#include "../GameSetting/GameSetting.h"
#include "../Camera/Camera.h"
#include "../Animation/Animation.h"

#define NYANTWO_MOVE_SPEED (0.5f)

// 変数
EnemyNyantwoData g_NyantwoData[ENEMY_NYANTWO_MAX] = { 0 };

// 画像ハンドル
int g_NyantwoHandle = 0;


void InitNyantwo()
{
	EnemyNyantwoData* two = g_NyantwoData;

	for (int i = 0; i < ENEMY_NYANTWO_MAX; i++, two++)
	{
		two->active = false;

		two->pos = VGet(0.0f, 0.0f, 0.0f);

		two->move = VGet(0.0f, 0.0f, 0.0f);

		two->handle = 0;
		two->isMoveChange = false;
		two->param = nullptr;
	}
}

void LoadNyantwo()
{
	g_NyantwoHandle = LoadGraph("Data/Effect/Nyantwo_top.png");

	for (int i = 0; i < ENEMY_NYANTWO_MAX; i++)
	{
		g_NyantwoData[i].handle = g_NyantwoHandle;
	}
}

void StarNyantwo()
{
}

void StepNyantwo()
{
	EnemyNyantwoData* two = g_NyantwoData;

	for (int i = 0; i < ENEMY_NYANTWO_MAX; i++, two++)
	{
		// 死んでいたら何もしない
		if (!two->active)
		{
			continue;
		}

		// 左に移動
		two->move.x = -NYANTWO_MOVE_SPEED;
	}
}

void UpdateNyantwo()
{
	EnemyNyantwoData* two = g_NyantwoData;

	for (int i = 0; i < ENEMY_NYANTWO_MAX; i++, two++)
	{
		if (!two->active)
		{
			continue;
		}

		two->pos.x += two->move.x;
	}
}

void DrawNyantwo()
{
	CameraData camera = GetCamera();

	EnemyNyantwoData* two = g_NyantwoData;

	// 歩行アニメーションの設定
	const int frameNum = 9;       // コマ数
	const float frameWidth = 48;    // 1コマの横幅
	const float frameHeight = 48;   // 1コマの縦幅
	const int interval = 8;       // 切り替え間隔

	// アニメーション用タイマー
	static int animTimer = 0;

	animTimer++;

	// 現在表示するコマ
	int animFrame = (animTimer / interval) % frameNum;

	for (int i = 0; i < ENEMY_NYANTWO_MAX; i++, two++)
	{
		if (!two->active)
		{
			continue;
		}

		// 切り取る画像のX座標
		int srcX = animFrame * frameWidth;

		// 歩行アニメーション描画
		DrawRectGraph((int)(two->pos.x - camera.pos.x),(int)(two->pos.y - camera.pos.y),
			srcX, 0, frameWidth, frameHeight, two->handle, TRUE);
	}
}

void FinNyantwo()
{
	DeleteGraph(g_NyantwoHandle);
}

void SpawnNyantwo(float x, float y)
{
	for (int i = 0; i < ENEMY_NYANTWO_MAX; i++)
	{
		EnemyNyantwoData* two = &g_NyantwoData[i];

		// 既に使われている
		if (two->active)
		{
			continue;
		}

		two->active = true;

		two->pos.x = x;
		two->pos.y = y -15.0f;
		two->pos.z = 0.0f;

		two->move.x = 0.0f;
		two->move.y = 0.0f;
		two->move.z = 0.0f;

		two->handle = g_NyantwoHandle;

		return;
	}
}

EnemyNyantwoData* GetNyantwo()
{
	return g_NyantwoData;
}

void TwoHitPlayer(int index)
{
	EnemyNyantwoData* two = &g_NyantwoData[index];

	if (!two->active) return;

	// 倒されたときのアニメーション
	PlayAnimationEffect(ENEMY_TWO_DEBT, two->pos.x, two->pos.y, 5);

	// 敵を消す
	two->active = false;
} 

void TwoHitPlayerBullet(int index)
{
	EnemyNyantwoData* two = &g_NyantwoData[index];

	if (!two->active) return;
	
	// 倒されたときのアニメーション
	PlayAnimationEffect(ENEMY_TWO_DEBT, two->pos.x, two->pos.y, 5);

	// 敵を消す
	two->active = false;
}
