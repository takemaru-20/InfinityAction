#pragma once
#include "AnimationParameter.h"
#include <DxLib.h>

//エフェクトの最大数
#define ANIMATION_MAX 255

//エフェクトデータ
struct AnimationData
{
	VECTOR pos;
	bool active;		//生存フラグ
	int handle;		//画像ハンドル
	int frameNum;		// フレーム数
	int frameWidth;		// 1コマの横幅
	int frameHeight;	// 1コマの縦幅
	int nowFrame;
	int timer;
	int interval;
};

//各種宣言
void InitAnimationEffect();
void LoadAnimationEffect();
void StepAnimationEffect();
void UpdateAnimationEffect();
void DrawAnimationEffect();
void FinAnimationEffect();

//エフェクト開始
void PlayAnimationEffect(Animation type, float posX, float posY, int interval);