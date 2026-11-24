
#pragma once
#include "../Collision/CollisionParameter.h"

void InitMap();
void LoadMap();
void StartMap();
void UpdateMap();
void DrawMap();
void FinMap();

void MoveWithMapCollision(Body* body);
