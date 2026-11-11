#include "DxLib.h"
#include "NyantwoJump.h"
#include "Enemyparameter.h"
#include "../GameSetting/GameSetting.h"
#include "../Camera/Camera.h"
#include "../Animation/Animation.h"
#include "../Map/MapManager.h"

#define NYANTWO_MOVE_SPEED (0.5f)
#define NYANTWO_GRAVITY (0.4f)
#define NYANTWO_JUMP_POWER (5.0f)

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
		two->JumpTimer = 3;
		two->handle = 0;
		two->isMoveChange = false;
		two->param = nullptr;
		two->isRight = false;
		two->onGround = false;

		two->frameNum = 9;
		two->frameWidth = 48;
		two->frameHeight = 48;
		two->interval = 8;
		two->animTimer = 0;

		two->Jumpdistance = 0;
		two->JumpTimer = 0;
		two->JumpSpeed = 0;
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

		// 移動方向
		if (two->isRight)
		{
			// 左に移動
			two->move.x = -NYANTWO_MOVE_SPEED;
		}
		else
		{
			// 右に移動
			two->move.x = NYANTWO_MOVE_SPEED;
		}

		// 重力
		two->move.y += NYANTWO_GRAVITY;

		// ジャンプまでの時間
		two->JumpTimer++;

		// 240フレームごとにジャンプ
		if (two->JumpTimer >= 240)
		{
			// 地面にいる場合だけジャンプ
			if (two->onGround)
			{
				two->move.y = -NYANTWO_JUMP_POWER;
				two->JumpTimer = 0;
				two->onGround = false;
			}
		}
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

		// 当たり判定用のデータ
		Body body = {};

		body.pos = two->pos;
		body.move = two->move;
		body.width = ENEMY_NYANTWO_WIDTH;
		body.height = ENEMY_NYANTWO_HEIGHT;

		// 最初は空中として扱う
		body.isAir = true;

		// ブロックとの当たり判定
		MoveWithMapCollision(&body);

		// 壁に当たったら方向転換
		if (two->move.x != 0.0f && body.move.x == 0.0f)
		{
			two->isRight = !two->isRight;
		}

		// 当たり判定後の座標を戻す
		two->pos = body.pos;
		two->move = body.move;

		// 地面に着地しているか
		two->onGround = !body.isAir;
	}
}


void DrawNyantwo()
{
	CameraData camera = GetCamera();

	EnemyNyantwoData* two = g_NyantwoData;

	two->animTimer++;

	// 現在表示するコマ
	int animFrame = (two->animTimer / two->interval) % two->frameNum;

	for (int i = 0; i < ENEMY_NYANTWO_MAX; i++, two++)
	{
		if (!two->active)
		{
			continue;
		}

		// 切り取る画像のX座標
		int srcX = animFrame * two->frameWidth;

		// 歩行アニメーション描画
		DrawRectGraph((int)(two->pos.x - camera.pos.x),(int)(two->pos.y - camera.pos.y + 11),
			srcX, 0, two->frameWidth, two->frameHeight, two->handle, TRUE, !two->isRight);
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
		two->isRight = false; 

		two->pos.x = x;
		two->pos.y = y;
		two->pos.z = 0.0f;

		two->JumpTimer = 0;
		two->JumpSpeed = 0.0f;
		two->onGround = false;

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
