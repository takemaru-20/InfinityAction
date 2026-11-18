#include "DxLib.h"
#include "Play.h"
#include "../../Player/Player.h"

//背景の横幅
#define PLAY_BG_WIDTH (1920.0f)

//背景が動く速さ
#define PLAYE_BG_SPPED (-1.5f)

PlayData g_PlayData = { 0 };

void InitPlayScene()
{
	g_PlayData.handele = 0;
	g_PlayData.pos.x = 0.0f;
}
void LoadPlayScene()
{
	g_PlayData.handele = LoadGraph("Data/Scene/grassland.png");
}

void StartPlayScene()
{
}

void StepPlayScene()
{
}

void UpdatePlayScene()
{
	PlayerData* player = GetPlayer();

	if (player->body.pos.x < 0.0f)
	{
		g_PlayData.pos.x += PLAYE_BG_SPPED;

		if (g_PlayData.pos.x <= -PLAY_BG_WIDTH)
		{
			g_PlayData.pos.x += PLAY_BG_WIDTH;
		}
	}
}

void DrawPlayScene()
{
	//1枚目の背景
	DrawGraph(g_PlayData.pos.x, 0, g_PlayData.handele, FALSE);

	//1枚目の右側に2枚目を書き、隙間をなくす
	DrawGraph((g_PlayData.pos.x + PLAY_BG_WIDTH),  0, g_PlayData.handele, FALSE);

}

void FinPlayScene()
{
	if (g_PlayData.handele != -1)
	{
		DeleteGraph(g_PlayData.handele);
		g_PlayData.handele = -1;
	}
}