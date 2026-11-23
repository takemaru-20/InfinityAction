#include "BulletManager.h"
#include "EnemyBullet.h"

void InitBullet()
{
    InitEnemyBullet();
}

void LoadBullet()
{
    LoadEnemyBullet();
}

void StepBullet()
{
    StepEnemyBullet();
}

void UpdateBullet()
{
    UpdateEnemyBullet();
}

void DrawBullet()
{
    DrawEnemyBullet();
}

void FinBullet()
{
    FinEnemyBullet();
}

void FireBullet(BulletCategory category, int typeID, FireBulletData fireData,
    BulletCollisionTag tag)
{
    if (category == BULLET_CATEGORY_STRAIGHT && typeID == STRAIGHT_BULLET_TYPE_CAN)
    {
        FireEnemyBullet(STRAIGHT_BULLET_TYPE_CAN, fireData, tag);
    }
}