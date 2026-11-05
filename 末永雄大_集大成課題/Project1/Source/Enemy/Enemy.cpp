#include "Enemy.h"
#include "Nyanone.h"
#include "NyantwoJump.h"

void InitEnemy()
{
	InitNyanone();
	InitNyantwo();
}

void LoadEnemy()
{
	LoadNyanone();
	LoadNyantwo();
}

void StarEnemy()
{
	StarNyanone();
	StarNyantwo();
}

void StepEnemy()
{
	StepNyanone();
	StepNyantwo();
}

void UpdateEnemy()
{
	UpdateNyanone();
	UpdateNyantwo();
}

void DrawEnemy()
{
	DrawNyanone();
	DrawNyantwo();
}

void FinEnemy()
{
	FinNyanone();
	FinNyantwo();
}

void StepEnemySpawnSystem()
{
}