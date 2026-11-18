#include "DxLib.h"
#include "Camera.h"
#include "../Input/Input.h"
#include "../Player/Player.h"
#include "../GameSetting/GameSetting.h"
#include "../Map/MapParameter.h"

#define DEBUG_CAMERA_SPEED (8.0f)
#define CAMERA_SCROLL_START_X (200.0f)

CameraData g_CameraData = { 0 };

void InitCamera()
{
	g_CameraData.pos.y = MAP_CHIP_Y_NUM * MAP_CHIP_HEIGHT - SCREEN_HEIGHT;
}

void StepCamera()
{
	PlayerData* player = GetPlayer();

	g_CameraData.pos.x = player->body.pos.x - 200.0f;
}

void DrawCamera()
{
}

CameraData GetCamera()
{
	return g_CameraData;
}
