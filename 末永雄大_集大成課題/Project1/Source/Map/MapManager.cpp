
#include "MapManager.h"
#include "MapParameter.h"
#include "MapChip.h"
#include "Block.h"
#include "../Camera/Camera.h"
#include <cmath>
#include "../Player/Player.h"
#include "../Collision/Collision.h"

// キャラクターの周囲何マスまでチェックするか
#define CHECK_ROUND_NUM (2)

namespace
{
	constexpr int CollapseColumn = 23;
	constexpr int CollapseRow = 17;
	constexpr unsigned int CollapseDelayMs = 1000;
	constexpr unsigned int CollapseAnimationMs = 450;

	enum class CollapseState { Ready, Waiting, Falling, Broken };

	CollapseState collapseState = CollapseState::Broken;
	unsigned int collapseStartedAt = 0;
	float collapseProgress = 0.0f;
	BlockData* collapseBlocks[9] = {};

	bool IsPlayerNearCollapse(const PlayerData& player)
	{
		if (!player.active) return false;

		const Body& body = player.body;
		for (const BlockData* block : collapseBlocks)
		{
			if (CheckSquareSquare(body.pos.x, body.pos.y, body.width, body.height,
				block->pos.x - MAP_CHIP_WIDTH, block->pos.y - MAP_CHIP_HEIGHT - 1.0f,
				3.0f * MAP_CHIP_WIDTH, 3.0f * MAP_CHIP_HEIGHT + 1.0f))
			{
				return true;
			}
		}
		return false;
	}
}

void InitMap()
{
	InitBlock();
}

void LoadMap()
{
	LoadMapChipData();
	LoadBlock();
}

void StartMap()
{
	CreateMap();

	collapseState = CollapseState::Broken;
	int blockIndex = 0;
	// 三角ブロックを起点に、下から順に1、3、5マスを登録する
	for (int rowAbove = 0; rowAbove <= 2; ++rowAbove)
	{
		const MapChipType expected = rowAbove == 0 ? ASNDTRIANGLE_BLOCK : ASND_BLOCK;
		for (int columnOffset = -rowAbove; columnOffset <= rowAbove; ++columnOffset)
		{
			const MapChipData chip = GetMapChipData(CollapseColumn + columnOffset, CollapseRow - rowAbove);
			if (chip.mapChip != expected || !chip.data) return;
			collapseBlocks[blockIndex++] = chip.data;
		}
	}
	collapseState = CollapseState::Ready;
}

void UpdateMap()
{
	if (collapseState == CollapseState::Broken) return;

	const unsigned int now = static_cast<unsigned int>(GetNowCount());
	const unsigned int elapsed = now - collapseStartedAt;
	if (collapseState == CollapseState::Falling)
	{
		if (elapsed >= CollapseAnimationMs)
		{
			collapseState = CollapseState::Broken;
		}
		else
		{
			collapseProgress = static_cast<float>(elapsed) / CollapseAnimationMs;
		}
		return;
	}

	if (!IsPlayerNearCollapse(*GetPlayer()))
	{
		collapseState = CollapseState::Ready;
		return;
	}

	if (collapseState == CollapseState::Ready)
	{
		collapseStartedAt = now;
		collapseState = CollapseState::Waiting;
		return;
	}

	if (elapsed >= CollapseDelayMs)
	{
		// 当たり判定を消し、倒れる間だけ別に描画する
		for (BlockData* block : collapseBlocks)
		{
			block->active = false;
		}
		collapseStartedAt = now;
		collapseProgress = 0.0f;
		collapseState = CollapseState::Falling;
	}
}

void DrawMap()
{
	DrawBlock();
	if (collapseState != CollapseState::Falling) return;

	const CameraData camera = GetCamera();
	const double angle = 1.57079632679 * collapseProgress;
	const float fall = 52.0f * collapseProgress * collapseProgress;
	const BlockData& bottom = *collapseBlocks[0];
	const float pivotX = bottom.pos.x + bottom.width * 0.5f;
	const float pivotY = bottom.pos.y + bottom.height;
	const double sine = std::sin(angle);
	const double cosine = std::cos(angle);
	for (const BlockData* block : collapseBlocks)
	{
		const float offsetX = block->pos.x + block->width * 0.5f - pivotX;
		const float offsetY = block->pos.y + block->height * 0.5f - pivotY;
		const double screenX = pivotX - camera.pos.x + offsetX * cosine - offsetY * sine;
		const double screenY = pivotY - camera.pos.y + fall + offsetX * sine + offsetY * cosine;
		DrawRotaGraph(static_cast<int>(screenX), static_cast<int>(screenY), 1.0, angle, block->handle, TRUE);
	}
}

void FinMap()
{
}

void MoveWithMapCollision(Body* body)
{
	// X軸だけ移動する
	body->pos.x += body->move.x;

	// マップチップと当たり判定＆押し出し
	for (int y = 0; y < MAP_CHIP_Y_NUM; y++)
	{
		for (int x = 0; x < MAP_CHIP_X_NUM; x++)
		{
			// マップチップ取得
			MapChipData mapChipData = GetMapChipData(x, y);

			// NORMAL_BLOCKとSOIL_BLOCK、GRASS_BLOCK以外は当たり判定しない
			if (mapChipData.mapChip != NORMAL_BLOCK && mapChipData.mapChip != SOIL_BLOCK
				&& mapChipData.mapChip != GRASS_BLOCK && mapChipData.mapChip != ASND_BLOCK
				&& mapChipData.mapChip != ASNDNORMAL_BLOCK)
			{
				continue;
			}

			// ブロックを取り出して当たり判定
			BlockData* block = mapChipData.data;

			// 念のためNULLチェック
			if (block == nullptr || !block->active)
			{
				continue;
			}

			// 当たり判定
			if (CheckSquareSquare(body->pos.x, body->pos.y, body->width, body->height,
				block->pos.x, block->pos.y, block->width, block->height))
			{
				// 左からあたったか
				if (body->move.x > 0.0f)
				{
					// 左に押し出す
					body->pos.x -= (body->pos.x + body->width) - block->pos.x;
				}

				// 右からあたったか
				else if (body->move.x < 0.0f)
				{
					// 右に押し出す
					body->pos.x += (block->pos.x + block->width) - body->pos.x;
				}

				// 移動量は0にする
				body->move.x = 0.0f;
			}
		}
	}

	// Y軸だけ移動する
	body->pos.y += body->move.y;

	// マップチップと当たり判定＆押し出し
	for (int y = 0; y < MAP_CHIP_Y_NUM; y++)
	{
		for (int x = 0; x < MAP_CHIP_X_NUM; x++)
		{
			// マップチップ取得
			MapChipData mapChipData = GetMapChipData(x, y);

			// NORMAL_BLOCKとSOIL_BLOCK、GRASS_BLOCK以外は当たり判定しない
			if (mapChipData.mapChip != NORMAL_BLOCK && mapChipData.mapChip != SOIL_BLOCK
				&& mapChipData.mapChip != GRASS_BLOCK && mapChipData.mapChip != ASND_BLOCK
				&& mapChipData.mapChip != ASNDNORMAL_BLOCK)
			{
				continue;
			}

			// ブロックを取り出して当たり判定
			BlockData* block = mapChipData.data;

			// 念のためNULLチェック
			if (block == nullptr || !block->active)
			{
				continue;
			}

			// 当たり判定
			if (CheckSquareSquare(body->pos.x, body->pos.y, body->width, body->height,
				block->pos.x, block->pos.y, block->width, block->height))
			{
				// 上からあたったか
				if (body->move.y > 0.0f)
				{
					// 上に押し出す
					body->pos.y -= (body->pos.y + body->height) - block->pos.y;

					// 空中フラグを折る
					body->isAir = false;
				}

				// 下からあたったか
				else if (body->move.y < 0.0f)
				{
					// 下に押し出す
					body->pos.y += (block->pos.y + block->height) - body->pos.y;
				}

				// 移動量は0にする
				body->move.y = 0.0f;
			}
		}
	}
}
