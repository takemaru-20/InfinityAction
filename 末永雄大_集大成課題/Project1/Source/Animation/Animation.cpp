#include "DxLib.h"
#include "Animation.h"
#include "AnimationParameter.h"
#include "../Camera/Camera.h"

//アニメーションエフェクトデータ
AnimationData g_AnimationData[ANIMATION_MAX] = { 0 };

//エフェクト画像ハンドル
int g_AnimationEffectHandle[ANIMATION_EFFECT_TYPE_MAX];

void InitAnimationEffect()
{
	AnimationData* effect = g_AnimationData;
	for (int i = 0; i < ANIMATION_MAX; i++, effect++)
	{
		effect->active = false;
		effect->handle = 0;
		effect->nowFrame = 0;
		effect->frameNum = 0;
		effect->frameWidth = 0;
		effect->frameHeight = 0;
		effect->timer = 0;
		effect->pos.x = 0;
		effect->pos.y = 0;
	}
}

void LoadAnimationEffect()
{
	g_AnimationEffectHandle[ENEMY_ONE_WAIK] = LoadGraph("Data/Effect/onewaik.png");
	g_AnimationEffectHandle[ENEMY_ONE_DEBT] = LoadGraph("Data/Effect/onedebt.png");
}

void StepAnimationEffect()
{
	AnimationData* effect = g_AnimationData;
	for (int i = 0; i < ANIMATION_MAX; i++, effect++)
	{
		if (!effect->active) continue;

		//次のコマに進めるか
		if (effect->timer <= 0)
		{
			//次のコマへ
			effect->nowFrame++;
			//タイマーリセット
			effect->timer = effect->interval;

			//前コマ表示し終わったら非アクティブ
			if (effect->nowFrame >= effect->frameNum)
			{
				effect->active = false;

				//これ以上処理する必要なし
				continue;
			}
		}

		effect->timer--;
	}
}

void UpdateAnimationEffect()
{
	
}

void DrawAnimationEffect()
{
	CameraData camera = GetCamera();

	AnimationData* effect = g_AnimationData;

	for (int i = 0; i < ANIMATION_MAX; i++, effect++)
	{
		if (!effect->active) continue;

		int srcX = effect->nowFrame * effect->frameWidth;
		int srcY = 0;

		DrawRectGraph(
			(int)(effect->pos.x - camera.pos.x - 22),
			(int)(effect->pos.y - camera.pos.y - 22),
			srcX,
			srcY,
			effect->frameWidth,
			effect->frameHeight,
			effect->handle,
			TRUE
		);
	}
}

void FinAnimationEffect()
{
	for (int i = 0; i < ANIMATION_EFFECT_TYPE_MAX; i++)
	{
		DeleteGraph(g_AnimationEffectHandle[i]);
	}
}

void PlayAnimationEffect(Animation type, float posX, float posY, int interval)
{
	// マスターパラム取得
	AnimatinParameter param = ANIM_EFFECT_MASTER_PARAM[type];

	AnimationData* effect = g_AnimationData;
	for (int i = 0; i < ANIMATION_MAX; i++, effect++)
	{
		// 未使用のエフェクトを探す
		if (!effect->active)
		{
			// アクティブ
			effect->active = true;

			// 座標設定
			effect->pos.x = posX;
			effect->pos.y = posY;

			// インターバル設定
			effect->interval = interval;

			// タイマー設定
			effect->timer = effect->interval;

			// 画像ハンドル
			effect->handle = g_AnimationEffectHandle[type];

			// 0コマ目にする
			effect->nowFrame = 0;

			// コマ数
			effect->frameNum = param.frameNum;

			// 1コマのサイズ
			effect->frameWidth = param.frameWidth;
			effect->frameHeight = param.frameHeight;

			break;
		}
	}
}
