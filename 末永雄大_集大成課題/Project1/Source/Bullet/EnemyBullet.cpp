#include "EnemyBullet.h"
#include "../Camera/Camera.h"

namespace
{
    constexpr int CAN_RADIUS = 6;
    constexpr int CAN_IMAGE_HALF_SIZE = 8;
    constexpr float CAN_GRAVITY = 0.35f;

    int g_CanBulletHandle = -1;
    StraightBulletData g_EnemyBullets[STRAIGHT_BULLET_MAX]{};
}

void InitEnemyBullet()
{
    for (StraightBulletData& bullet : g_EnemyBullets)
    {
        bullet = {};
    }
}

void LoadEnemyBullet()
{
    g_CanBulletHandle = LoadGraph("Data/Bullet/EnemyBullet.png");
}

void StepEnemyBullet()
{
    for (StraightBulletData& bullet : g_EnemyBullets)
    {
        if (bullet.active)
        {
            bullet.life--;
        }
    }
}

void UpdateEnemyBullet()
{
    for (StraightBulletData& bullet : g_EnemyBullets)
    {
        if (!bullet.active)
        {
            continue;
        }

        if (bullet.life <= 0)
        {
            bullet.active = false;
            continue;
        }

        bullet.body.pos.x += bullet.body.move.x;
        bullet.body.pos.y += bullet.body.move.y;
        bullet.body.move.y += CAN_GRAVITY;
    }
}

void DrawEnemyBullet()
{
    CameraData camera = GetCamera();
    unsigned int color = GetColor(180, 180, 180);

    for (const StraightBulletData& bullet : g_EnemyBullets)
    {
        if (!bullet.active)
        {
            continue;
        }

        int x = static_cast<int>(bullet.body.pos.x - camera.pos.x);
        int y = static_cast<int>(bullet.body.pos.y - camera.pos.y);

        if (g_CanBulletHandle != -1)
        {
            DrawExtendGraph(
                x - CAN_IMAGE_HALF_SIZE,
                y - CAN_IMAGE_HALF_SIZE,
                x + CAN_IMAGE_HALF_SIZE,
                y + CAN_IMAGE_HALF_SIZE,
                g_CanBulletHandle,
                TRUE);
        }
        else
        {
            // 画像を読み込めなかった場合は灰色の丸を描く
            DrawCircle(x, y, CAN_RADIUS, color, TRUE);
        }
    }
}

void FinEnemyBullet()
{
    if (g_CanBulletHandle != -1)
    {
        DeleteGraph(g_CanBulletHandle);
        g_CanBulletHandle = -1;
    }
}

void FireEnemyBullet(StraightBulletType type, FireBulletData fireData,
    BulletCollisionTag tag)
{
    if (type != STRAIGHT_BULLET_TYPE_CAN || fireData.life <= 0)
    {
        return;
    }

    for (StraightBulletData& bullet : g_EnemyBullets)
    {
        if (bullet.active)
        {
            continue;
        }

        bullet = {};
        bullet.body.pos = fireData.pos;
        bullet.body.move = fireData.move;
        bullet.body.width = CAN_RADIUS * 2.0f;
        bullet.body.height = CAN_RADIUS * 2.0f;
        bullet.body.radius = CAN_RADIUS;
        bullet.life = fireData.life;
        bullet.tag = tag;
        bullet.active = true;
        return;
    }
}

StraightBulletData* GetEnemyBullet()
{
    return g_EnemyBullets;
}

void EnemyBulletHitEnemy(int index)
{
    g_EnemyBullets[index].active = false;
}

void EnemyBulletHitPlayer(int index)
{
    g_EnemyBullets[index].active = false;
}