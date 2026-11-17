#include "Mouse.h"
#include "../Scene/SceneManager.h"
#include "../GameSetting/GameSetting.h"

MosuePoint NowPoint;		//現在のマウスの位置
MosuePoint OldPoint;		//以前のマウスの位置

int NowMousePressFrame[MouseKinMax];	//現在のマウスのボタンを押しているフレーム数を管理
int OldMousePressFrame[MouseKinMax];	//以前のマウスのボタンを押しているフレーム数を管理

int NowWheelValue;	//現在のマウスのホイール回転量
int OldWheelValue;	//以前のマウスのホイール回転量

//マウスのボタンコードとフレーム数を管理している配列の要素数を紐付ける
int MouseCodeIndex[MouseKinMax]
{
	MOUSE_INPUT_LEFT,	//配列の要素数0にはマウスのボタン情報0×0001の情報を入れる
	MOUSE_INPUT_RIGHT,	//配列の要素数1にはマウスのボタン情報0×0002の情報を入れる
	MOUSE_INPUT_MIDDLE,	//配列の要素数2にはマウスのボタン情報0×0004の情報を入れる
	MOUSE_INPUT_4,		//配列の要素数3にはマウスのボタン情報0×0008の情報を入れる
	MOUSE_INPUT_6,		//配列の要素数4にはマウスのボタン情報0×0010の情報を入れる
	MOUSE_INPUT_4,		//配列の要素数5にはマウスのボタン情報0×0020の情報を入れる
	MOUSE_INPUT_7,		//配列の要素数6にはマウスのボタン情報0×0040の情報を入れる
	MOUSE_INPUT_8,		//配列の要素数7にはマウスのボタン情報0×0080の情報を入れる
};

//Now???系の変数の値をOld???系の変数へ入れる
void MouseNowIntoOld(VOID)
{
	OldPoint = NowPoint;	//マウスの位置

	for (int i = 0; i < MouseKinMax; i++)
	{
		OldMousePressFrame[i] = NowMousePressFrame[i];
	}

	//ホイール量
	OldWheelValue = NowWheelValue;

	return;
}

//マウスのボタンコードを配列の要素数に変換する
int MouseCodeTpIndex(int MOUSE_INPUT_)
{
	for (int i = 0; i < MouseKinMax; i++)
	{
		//マウスコードが紐付けされているなら
		if (MouseCodeIndex[i] == MOUSE_INPUT_)
		{
			return i;
		}
	}

	//マウスコードが無いならエラー
	return MouseCodeErrIndex;
}

void InitMouse()
{
	//マウスの位置
	NowPoint.pos.x = 0;
	NowPoint.pos.y = 0;

	//フレーム数
	for (int i = 0; i < MouseKinMax; i++)
	{
		NowMousePressFrame[i] = 0;
	}

	//ホイール量
	NowWheelValue = 0;

	//Old系も初期化
	MouseNowIntoOld();

	return;
}

void UpdateMouse()
{
	//マウスのボタン情報を全て取得
	int Input;

	//現在の情報を以前の情報として保存
	MouseNowIntoOld();

	int mouseX;
	int mouseY;

	GetMousePoint(&mouseX, &mouseY);

	NowPoint.pos.x = mouseX;
	NowPoint.pos.y = mouseY;

	//もし、マウスの座標がゲーム画面外にあるなら、ゲーム画面内に収める
	if (NowPoint.pos.x < 0) { NowPoint.pos.x = 0; }	//左
	else if (NowPoint.pos.x > SCREEN_WIDTH) {NowPoint.pos.x = SCREEN_WIDTH;}	//右

	if (NowPoint.pos.y < 0) { NowPoint.pos.y = 0; }	//左
	else if (NowPoint.pos.y > SCREEN_WIDTH) { NowPoint.pos.y = SCREEN_WIDTH; }	//右

	//マウスのボタン情報を一気に登録
	Input = GetMouseInput();

	//各ボタンを押しているかチェック
	for (int i = 0; i < MouseKinMax; i++)
	{

	}

	return;

}
