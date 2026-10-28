#include"DxLib.h" 
#include "Map/MapManager.h"
#include "Player/Player.h"
#include "Input/Input.h"
#include "Camera/Camera.h"

//プログラムの初めはWinMainから始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstamce, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	if (DxLib_Init() == -1)  //DXライブラリ初期化処理
	{
		return -1;           //エラーが起きたら直ちに終了
	}
	InitMap();
	InitPlayer();
	InitCamera();
	
	LoadMap();
	LoadPlayer();

	StartPlayer();
	
	//描画先を裏画面にする
	SetDrawScreen (DX_SCREEN_BACK);

    //ゲームのメインループ
	while (ProcessMessage() >= 0)
	{
		//画面をクリア
		ClearDrawScreen();

		StartMap();
		StepPlayer();
		StepCamera();

		UpdatePlayer();
		UpdateInput();
		
        DrawMap();
		DrawPlayer();
		DrawCamera();

		//エスケープキーで終了
		if (CheckHitKey(KEY_INPUT_ESCAPE)) break;

		//画面フリップ
		ScreenFlip();
	}

	FinPlayer();

	DxLib_End();     //DXライブラリの終了処理

	return 0;       //ソフト終了
}