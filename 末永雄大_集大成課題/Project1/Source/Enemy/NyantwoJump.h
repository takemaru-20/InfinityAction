#pragma once
#include "Enemyparameter.h"

// プロトタイプ宣言
void InitNyantwo();
void LoadNyantwo();
void StarNyantwo();
void StepNyantwo();
void UpdateNyantwo();
void DrawNyantwo();
void FinNyantwo();

// マップから敵を生成
void SpawnNyantwo(float x, float y);

EnemyNyantwoData* GetNyantwo();

//当たり判定
void TwoHitPlayer(int index);
void TwoHitPlayerBullet(int index);