#pragma once
#include "BulletParameter.h"

void InitBullet();
void LoadBullet();
void StepBullet();
void UpdateBullet();
void DrawBullet();
void FinBullet();

//弾発射
void FireBullet(BulletCategory category, int typeID, FireBulletData fireData, BulletCollisionTag tag);