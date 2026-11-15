#include "Enemy.h"
#include "Nyanone.h"
#include "NyantwoJump.h"
#include "Canthrowing.h"

void InitEnemy()
{
	InitNyanone();
	InitNyantwo();
	InitCan();
}

void LoadEnemy()
{
	LoadNyanone();
	LoadNyantwo();
	LoadCan();
}

void StarEnemy()
{
	StarNyanone();
	StarNyantwo();
	StarCan();
}

void StepEnemy()
{
	StepNyanone();
	StepNyantwo();
	StepCan();
}

void UpdateEnemy()
{
	UpdateNyanone();
	UpdateNyantwo();
	UpdateCan();
}

void DrawEnemy()
{
	DrawNyanone();
	DrawNyantwo();
	DrawCan();
}

void FinEnemy()
{
	FinNyanone();
	FinNyantwo();
	FinCan();
}

void StepEnemySpawnSystem()
{
}