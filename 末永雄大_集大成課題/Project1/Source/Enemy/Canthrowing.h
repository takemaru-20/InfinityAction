#pragma once
#include "Enemyparameter.h"

// プロトタイプ宣言
void InitCan();
void LoadCan();
void StarCan();
void StepCan();
void UpdateCan();
void DrawCan();
void FinCan();

// マップから敵を生成
void SpawnCan(float x, float y);

EnemyCanData* GetCan();

//当たり判定
void CanHitPlayer(int index);
void CanHitPlayerBullet(int index);