#pragma once
#include "BulletParameter.h"

// 各ロジックごとの関数
void InitEnemyBullet();
void LoadEnemyBullet();
void StepEnemyBullet();
void UpdateEnemyBullet();
void DrawEnemyBullet();
void FinEnemyBullet();

// 発射関数（必ずBulletManagerでしか呼ばないこと）
void FireEnemyBullet(StraightBulletType type, FireBulletData fireData, BulletCollisionTag tag);

// 弾取得
StraightBulletData* GetEnemyBullet();

// 当たり判定関連
void EnemyBulletHitEnemy(int index);
void EnemyBulletHitPlayer(int index);
