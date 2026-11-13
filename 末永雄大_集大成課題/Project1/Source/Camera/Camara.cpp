#include "DxLib.h"
#include "Camera.h"
#include "../Input/Input.h"
#include "../Player/Player.h"

#define DEBUG_CAMERA_SPEED (8.0f)
#define CAMERA_SCROLL_START_X (200.0f)

CameraData g_CameraData = { 0 };

void InitCamera()
{

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
