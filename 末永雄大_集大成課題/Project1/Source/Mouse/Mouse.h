#pragma once
#include "DxLib.h"

//マクロ定義
#define MouseKinMax			  8 //マウスのボタンの種類
#define MouseCodeErrIndex	999	//マウスコードを変換した要素数のエラー


//構造体
struct MosuePoint
{
	VECTOR pos; //マウスのXYの位置

} MousePoint;

//外部のプロトタイプ宣言
void MouseNowIntoOld(VOID);				//Now???系の変数の値をOld???系の変数へ入れる
int MouseCodeTpIndex(int MOUSE_INPUT_);  //マウスのボタンコードを配列の要素に変換する

void InitMouse();
void UpdateMouse();

bool MouseDown(int MOUSE_INPUT_);		//特定のボタンを押したか
bool MouseClick(int MOUSE_INPUT_);		//特定のボタンをクリックしたか
int MousePressFrame(int MOUSE_INPUT_);	//特定のボタンを押したフレーム数

MosuePoint GetPointMouse(VOID);			//マウスの現在の位置を取得する
MosuePoint GetOldPointMouse(VOID);		//マウスの以前の位置を取得する
MosuePoint GetDiffPointMouse(VOID);		//マウスの依然と現在の位置の差を取得する

int GetWheeMouse(VOID);					//マウスの現在のホイール量を取得する