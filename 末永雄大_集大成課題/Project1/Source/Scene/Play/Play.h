#pragma once

struct PlayData
{
	int handele;
	VECTOR pos;
};

void InitPlayScene();
void LoadPlayScene();
void StartPlayScene();
void StepPlayScene();
void UpdatePlayScene();
void DrawPlayScene();
void FinPlayScene();