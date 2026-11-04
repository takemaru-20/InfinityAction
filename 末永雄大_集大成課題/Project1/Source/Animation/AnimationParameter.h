#pragma once

//エフェクト種類
enum Animation
{
	ENEMY_ONE_WAIK,
	ENEMY_ONE_DEBT,

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
	3,  32, 32,	// ENEMY_ONE_WAIK
	8, 80, 80,	// ENEMY_DEAD
};