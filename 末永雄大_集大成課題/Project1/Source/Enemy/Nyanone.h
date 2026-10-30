#pragma once
#include "Enemyparameter.h"

// プロトタイプ宣言
void InitNyanone();
void LoadNyanone();
void StarNyanone();
void StepNyanone();
void UpdateNyanone();
void DrawNyanone();
void FinNyanone();

// マップから敵を生成
void SpawnNyanone(float x, float y);

EnemyNyanoneData* GetNyanone();

//当たり判定
void oneHitPlayer(int index);
void oneHitPlayerBullet(int index);