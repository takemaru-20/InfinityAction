#include "DxLib.h"
#include "Collision.h"
#include "../Player/Player.h"
#include "../Map/Block.h"
#include "../Map/MapManager.h"
#include <math.h>
#include "../Enemy/Enemyparameter.h"
#include "../Enemy/Nyanone.h"
#include "../Enemy/NyantwoJump.h"
#include "../Enemy/Canthrowing.h"
#include "../Bullet/EnemyBullet.h"

// 矩形と点の当たり判定
bool CheckSquarePoint(float squarePosX, float squarePosY,
    float squareWidth, float squareHeight, float pointX, float pointY)
{
    // 点のX座標が矩形の間にあるか
    if (pointX >= squarePosX && pointX <= squarePosX + squareWidth)
    {
        // 点のY座標が矩形の間にあるか
        if (pointY >= squarePosY && pointY <= squarePosY + squareHeight)
        {
            return true;
        }
    }

    return false;
}

// 矩形と矩形の当たり判定
bool CheckSquareSquare(float squareA_PosX, float squareA_PosY,
    float squareA_Width, float squareA_Height, float squareB_PosX,
    float squareB_PosY, float squareB_Width, float squareB_Height)
{
    // 矩形Aの各辺の位置
    float leftA = squareA_PosX;
    float rightA = squareA_PosX + squareA_Width;
    float topA = squareA_PosY;
    float bottomA = squareA_PosY + squareA_Height;

    // 矩形Bの各辺の位置
    float leftB = squareB_PosX;
    float rightB = squareB_PosX + squareB_Width;
    float topB = squareB_PosY;
    float bottomB = squareB_PosY + squareB_Height;

    // 左右と上下の両方が重なっているか
    if (rightA > leftB && leftA < rightB)
    {
        if (bottomA > topB && topA < bottomB)
        {
            return true;
        }
    }

    return false;
}

// 円と点の当たり判定
bool CheckCirclePoint(float circlePosX, float circlePosY,
    float circleRadius, float pointX, float pointY)
{
    // 円の中心から点までの距離
    float distance = sqrtf(
        (pointX - circlePosX) * (pointX - circlePosX)
        + (pointY - circlePosY) * (pointY - circlePosY));

    if (distance <= circleRadius)
    {
        return true;
    }

    return false;
}

// 円と円の当たり判定
bool CheckCircleCircle(float circleA_PosX, float circleA_PosY,
    float circleA_Radius, float circleB_PosX, float circleB_PosY,
    float circleB_Radius)
{
    // 2つの円の中心間の距離
    float distance = sqrtf(
        (circleB_PosX - circleA_PosX) * (circleB_PosX - circleA_PosX)
        + (circleB_PosY - circleA_PosY) * (circleB_PosY - circleA_PosY));

    if (distance <= circleA_Radius + circleB_Radius)
    {
        return true;
    }

    return false;
}

void CheckPlayerone()
{
    PlayerData* player = GetPlayer();
    EnemyNyanoneData* one = GetNyanone();

    // プレイヤーが生きていて、無敵時間中でないとき
    if (player->active && !player->inviibie)
    {
        for (int i = 0; i < ENEMY_NYANONE_MAX; i++, one++)
        {
            if (!one->active)
            {
                continue;
            }

            if (CheckSquareSquare(
                player->body.pos.x, player->body.pos.y,
                PLAYER_WIDTH, PLAYER_HEIGHT,
                one->pos.x, one->pos.y,
                ENEMY_NYANONE_WIDTH, ENEMY_NYANONE_HEIGHT))
            {
                float playerBottom = player->body.pos.y + PLAYER_HEIGHT;
                float enemyTop = one->pos.y;

                // プレイヤーが落下中で、敵の上側にいる
                if (player->body.move.y > 0 && playerBottom < enemyTop + 10)
                {
                    OneHitPlayer(i);
                    player->body.move.y = -8;
                }
                else
                {
                    PlayerHitEnemy();
                    return;
                }
            }
        }
    }
}

void CheckPlayertwo()
{
    PlayerData* player = GetPlayer();
    EnemyNyantwoData* two = GetNyantwo();

    // プレイヤーが生きていて、無敵時間中でないとき
    if (player->active && !player->inviibie)
    {
        for (int i = 0; i < ENEMY_NYANTWO_MAX; i++, two++)
        {
            if (!two->active)
            {
                continue;
            }

            if (CheckSquareSquare(
                player->body.pos.x, player->body.pos.y,
                PLAYER_WIDTH, PLAYER_HEIGHT,
                two->pos.x, two->pos.y,
                ENEMY_NYANTWO_WIDTH, ENEMY_NYANTWO_HEIGHT))
            {
                float playerBottom = player->body.pos.y + PLAYER_HEIGHT;
                float enemyTop = two->pos.y;

                // プレイヤーが落下中で、敵の上側にいる
                if (player->body.move.y > 0 && playerBottom < enemyTop + 10)
                {
                    TwoHitPlayer(i);
                    player->body.move.y = -8;
                }
                else
                {
                    PlayerHitEnemy();
                    return;
                }
            }
        }
    }
}

// プレイヤーと缶を投げる敵本体の当たり判定
void CheckPlayerCan()
{
    PlayerData* player = GetPlayer();
    EnemyCanData* can = GetCan();

    if (player->active && !player->inviibie)
    {
        for (int i = 0; i < ENEMY_CAN_MAX; i++, can++)
        {
            if (!can->active)
            {
                continue;
            }

            if (CheckSquareSquare(
                player->body.pos.x, player->body.pos.y,
                PLAYER_WIDTH, PLAYER_HEIGHT,
                can->pos.x, can->pos.y,
                ENEMY_CAN_WIDTH, ENEMY_CAN_HEIGHT))
            {
                float playerBottom = player->body.pos.y + PLAYER_HEIGHT;
                float enemyTop = can->pos.y;

                // プレイヤーが落下中で、敵の上側にいる
                if (player->body.move.y > 0 && playerBottom < enemyTop + 10)
                {
                    CanHitPlayer(i);
                    player->body.move.y = -8;
                }
                else
                {
                    PlayerHitEnemy();
                    return;
                }
            }
        }
    }
}

// プレイヤーと飛んでいる敵の弾の当たり判定
static void CheckPlayerEnemyBullet()
{
    PlayerData* player = GetPlayer();
    if (!player->active || player->inviibie)
    {
        return;
    }

    StraightBulletData* bullet = GetEnemyBullet();

    for (int i = 0; i < STRAIGHT_BULLET_MAX; i++, bullet++)
    {
        if (!bullet->active || bullet->tag != BULLET_COLLISION_TAG_ENEMY)
        {
            continue;
        }

        // 弾の中心座標から、当たり判定に使う左上の座標を求める
        float bulletLeft = bullet->body.pos.x - bullet->body.width * 0.5f;
        float bulletTop = bullet->body.pos.y - bullet->body.height * 0.5f;

        if (CheckSquareSquare(
            player->body.pos.x, player->body.pos.y,
            PLAYER_WIDTH, PLAYER_HEIGHT,
            bulletLeft, bulletTop,
            bullet->body.width, bullet->body.height))
        {
            // 当たった弾を消して、プレイヤーの被弾処理を呼ぶ
            EnemyBulletHitPlayer(i);
            PlayerHitEnemyBullet();
            return;
        }
    }
}

void CheckCollison()
{
    CheckPlayerone();
    CheckPlayertwo();
    CheckPlayerCan();
    CheckPlayerEnemyBullet();
}