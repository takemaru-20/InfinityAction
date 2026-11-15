#pragma once

//エフェクト種類
enum Animation
{
	ENEMY_ONE_DEBT,
	ENEMY_TWO_DEBT,
	ENEMY_CAN_DEBT,

	ANIMATION_EFFECT_TYPE_MAX,
};

//エフェクトのパラメータ
struct AnimatinParameter
{
	int frameNum;		// フレーム数
	int frameWidth;		// 1コマの横幅
	int frameHeight;	// 1コマの縦幅
};

const AnimatinParameter ANIM_EFFECT_MASTER_PARAM[ANIMATION_EFFECT_TYPE_MAX] =
{
	8, 80, 80,  // oneの死亡アニメーション
	8, 80, 80,   // twoの死亡アニメーション
	8, 80, 80,	//canの死亡アニメーション
};