#include "DxLib.h"
#include "Nyanone.h"
#include "Enemyparameter.h"
#include "../GameSetting/GameSetting.h"
#include "../Camera/Camera.h"
#include "../Animation/Animation.h"
#include "../Collision/Collision.h"
#include "../Map/MapManager.h"
#include "../Map/MapChip.h"
#include "../Map/MapParameter.h"
#include "../Sound/SoundManager.h"

#define NYANONE_MOVE_SPEED (0.5f)
#define NYANONE_GRAVITY (0.4f)

// 変数
EnemyNyanoneData g_NyanoneData[ENEMY_NYANONE_MAX] = { 0 };

// 画像ハンドル
int g_NyanoneHandle = 0;

void InitNyanone()
{
    EnemyNyanoneData* one = g_NyanoneData;

    for (int i = 0; i < ENEMY_NYANONE_MAX; i++, one++)
    {
        one->active = false;
        one->pos = VGet(0.0f, 0.0f, 0.0f);
        one->move = VGet(0.0f, 0.0f, 0.0f);
        one->handle = 0;
        one->isMoveChange = false;
        one->isRight = false;
        one->param = nullptr;

        one->frameNum = 8;
        one->frameWidth = 37;
        one->frameHeight = 37;
        one->interval = 8;
        one->animTimer = 0;
    }
}

void LoadNyanone()
{
    g_NyanoneHandle = LoadGraph("Data/Effect/onewaik.png");

    for (int i = 0; i < ENEMY_NYANONE_MAX; i++)
    {
        g_NyanoneData[i].handle = g_NyanoneHandle;
    }
}

void StarNyanone()
{
}

void StepNyanone()
{
    EnemyNyanoneData* one = g_NyanoneData;

    for (int i = 0; i < ENEMY_NYANONE_MAX; i++, one++)
    {
        // 死んでいたら何もしない
        if (!one->active)
        {
            continue;
        }

        // 移動方向
        if (one->isRight)
        {
            // 左に移動
            one->move.x = -NYANONE_MOVE_SPEED;
        }
        else
        {
            // 右に移動
            one->move.x = NYANONE_MOVE_SPEED;
        }

        // 重力
        one->move.y += NYANONE_GRAVITY;
    }
}

// 進行方向に床があるか確認
bool CheckNyanoneFloor(float x, float y)
{
    for (int mapY = 0; mapY < MAP_CHIP_Y_NUM; mapY++)
    {
        for (int mapX = 0; mapX < MAP_CHIP_X_NUM; mapX++)
        {
            MapChipData chip = GetMapChipData(mapX, mapY);

            // 床ブロック以外は無視
            if (chip.mapChip != NORMAL_BLOCK &&
                chip.mapChip != SOIL_BLOCK &&
                chip.mapChip != GRASS_BLOCK)
            {
                continue;
            }

            BlockData* block = chip.data;

            if (block == nullptr)
            {
                continue;
            }

            if (x >= block->pos.x &&
                x < block->pos.x + block->width &&
                y >= block->pos.y &&
                y < block->pos.y + block->height)
            {
                return true;
            }
        }
    }

    return false;
}

void UpdateNyanone()
{
    EnemyNyanoneData* one = g_NyanoneData;

    for (int i = 0; i < ENEMY_NYANONE_MAX; i++, one++)
    {
        if (!one->active)
        {
            continue;
        }

        Body body = {};

        body.pos = one->pos;
        body.move = one->move;
        body.width = ENEMY_NYANONE_WIDTH;
        body.height = ENEMY_NYANONE_HEIGHT;

        // ブロックとの当たり判定
        MoveWithMapCollision(&body);

        // 壁に当たったか確認
        bool hitWall =
            (one->move.x != 0.0f && body.move.x == 0.0f);

        // 座標を戻す
        one->pos = body.pos;
        one->move = body.move;

        // 足元の床を確認
        float footY = one->pos.y + body.height + 1.0f;
        float centerX = one->pos.x + body.width / 2.0f;

        // 現在の足元に床があるか
        bool onFloor = CheckNyanoneFloor(centerX, footY);

        // 進行方向の床を調べる
        float checkX;

        if (one->isRight)
        {
            // 左方向の床を確認
            checkX = one->pos.x - 8.0f;
        }
        else
        {
            // 右方向の床を確認
            checkX = one->pos.x + body.width + 8.0f;
        }

        // 壁に当たったら反転
        if (hitWall)
        {
            one->isRight = !one->isRight;
        }

        // 地面に立っていて、進行方向に床がなければ反転
        else if (onFloor && !CheckNyanoneFloor(checkX, footY))
        {
            one->isRight = !one->isRight;
        }
    }
}

void DrawNyanone()
{
    CameraData camera = GetCamera();

    EnemyNyanoneData* one = g_NyanoneData;

    one->animTimer++;

    // 現在表示するコマ
    int animFrame = (one->animTimer / one->interval) % one->frameNum;

    for (int i = 0; i < ENEMY_NYANONE_MAX; i++, one++)
    {
        if (!one->active)
        {
            continue;
        }

        // 切り取る画像のX座標
        int srcX = animFrame * one->frameWidth;

        // 歩行アニメーション描画
        DrawRectGraph(
            (int)(one->pos.x - camera.pos.x),
            (int)(one->pos.y - camera.pos.y + 11),
            srcX, 0,
            one->frameWidth,
            one->frameHeight,
            one->handle,
            TRUE,
            !one->isRight
        );
    }
}

void FinNyanone()
{
    DeleteGraph(g_NyanoneHandle);
}

void SpawnNyanone(float x, float y)
{
    for (int i = 0; i < ENEMY_NYANONE_MAX; i++)
    {
        EnemyNyanoneData* one = &g_NyanoneData[i];

        // 既に使われている
        if (one->active)
        {
            continue;
        }

        one->active = true;
        one->isRight = false;

        one->pos.x = x;
        one->pos.y = y;
        one->pos.z = 0.0f;

        one->move.x = 0.0f;
        one->move.y = 0.0f;
        one->move.z = 0.0f;

        one->handle = g_NyanoneHandle;

        return;
    }
}

EnemyNyanoneData* GetNyanone()
{
    return g_NyanoneData;
}

void OneHitPlayer(int index)
{
    EnemyNyanoneData* one = &g_NyanoneData[index];

    if (!one->active) return;

    // 敵の死亡音を再生
    PlaySE(SE_ENEMY_DEAD);

    // 倒されたときのアニメーション
    PlayAnimationEffect(ENEMY_ONE_DEBT, one->pos.x, one->pos.y, 5);

    // 敵を消す
    one->active = false;
}

void OneHitPlayerBullet(int index)
{
    EnemyNyanoneData* one = &g_NyanoneData[index];

    if (!one->active) return;

    // 敵の死亡音を再生
    PlaySE(SE_ENEMY_DEAD);

    // 倒されたときのアニメーション
    PlayAnimationEffect(ENEMY_ONE_DEBT, one->pos.x, one->pos.y, 5);

    // 敵を消す
    one->active = false;
}
