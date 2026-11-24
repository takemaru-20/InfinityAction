#include "DxLib.h"
#include "Map/MapManager.h"
#include "Player/Player.h"
#include "Input/Input.h"
#include "Camera/Camera.h"
#include "Enemy/Enemy.h"
#include "Bullet/BulletManager.h"
#include "Collision/Collision.h"
#include "Animation/Animation.h"
#include "FPS/FPS.h"
#include "GameSetting/GameSetting.h"
#include "Scene/Play/Play.h"
#include "Sound/SoundManager.h"

// プログラムの初めはWinMainから始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstamce,
    _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    SetOutApplicationLogValidFlag(FALSE); // Log.txtを出力しない
    SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_COLOR_DEPTH);

    // ChangeWindowMode(TRUE); // ウィンドウモードに設定（完成時消す）

    if (DxLib_Init() == -1)
    {
        return -1;
    }

    InitPlayScene();
    InitMap();
    InitPlayer();
    InitCamera();
    InitEnemy();
    InitBullet();
    InitAnimationEffect();
    InitFPS();

    LoadPlayScene();
    LoadMap();
    LoadPlayer();
    LoadEnemy();
    LoadBullet();
    LoadAnimationEffect();
    LoadBGM();
    LoadSE();

    StartPlayer();
    StarEnemy();
    StartMap();

    // 描画先を裏画面にする
    SetDrawScreen(DX_SCREEN_BACK);

    // ゲームのメインループ
    while (ProcessMessage() >= 0)
    {
        ClearDrawScreen();

        CheckCollison();

        StepPlayer();
        StepCamera();
        StepEnemy();
        StepBullet();
        StepAnimationEffect();

        UpdatePlayScene();
        UpdatePlayer();
        UpdateMap();
        UpdateInput();
        UpdateEnemy();
        UpdateBullet();
        UpdateFPS();

        DrawPlayScene();
        DrawMap();
        DrawPlayer();
        DrawCamera();
        DrawEnemy();
        DrawBullet();
        DrawAnimationEffect();
        DrawFPS();

        if (CheckHitKey(KEY_INPUT_ESCAPE))
        {
            break;
        }

        ScreenFlip();
        FPSWait();
    }

    FinPlayScene();
    FinPlayer();
    FinEnemy();
    FinBullet();
    FinAnimationEffect();
    FinBGM();
    FinSE();

    DxLib_End();
    return 0;
}