#include "MapManager.h"
#include "MapParameter.h"
#include "MapChip.h"
#include "Block.h"
#include "../Collision/Collision.h"
#include "../Player/Player.h"
#include "../Camera/Camera.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <vector>

// キャラクターの周囲何マスまでチェックするか
#define CHECK_ROUND_NUM (2)

// 流砂の沈む速さ
#define QUICKSAND_SINK_SPEED (0.8f)

// 崩れる砂ブロックの時間（ミリ秒）
#define COLLAPSE_TIME (1000)

namespace
{
	constexpr unsigned int COLLAPSE_FALL_TIME = 450;

	enum class CollapseState { Ready, Waiting, Falling, Finished };

	struct CollapseGroup
	{
		std::array<BlockData*, 9> blocks = {};
		CollapseState state = CollapseState::Ready;
		unsigned int startedAt = 0;
		float progress = 0.0f;
	};

	std::vector<CollapseGroup> g_CollapseGroups;
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
	g_CollapseGroups.clear();

	for (int y = 2; y < MAP_CHIP_Y_NUM; ++y)
	{
		for (int x = 2; x < MAP_CHIP_X_NUM - 2; ++x)
		{
			const MapChipData base = GetMapChipData(x, y);
			if (base.mapChip != ASNDTRIANGLE_BLOCK || !base.data || !base.data->active)
			{
				continue;
			}

			CollapseGroup group;
			group.blocks[0] = base.data;
			int count = 1;

			// 10番の1段上に9番が3個、2段上に8番が5個ある組を登録する
			for (int rowAbove = 1; rowAbove <= 2; ++rowAbove)
			{
				const MapChipType expected = rowAbove == 1 ? ASNDNORMAL_BLOCK : ASND_BLOCK;
				for (int offset = -rowAbove; offset <= rowAbove; ++offset)
				{
					const MapChipData chip = GetMapChipData(x + offset, y - rowAbove);
					if (chip.mapChip == expected && chip.data && chip.data->active)
					{
						group.blocks[count++] = chip.data;
					}
				}
			}

			if (count == group.blocks.size())
			{
				g_CollapseGroups.push_back(group);
			}
		}
	}
}

void UpdateMap()
{
	const unsigned int now = static_cast<unsigned int>(GetNowCount());
	for (CollapseGroup& group : g_CollapseGroups)
	{
		const unsigned int elapsed = now - group.startedAt;
		if (group.state == CollapseState::Waiting && elapsed >= COLLAPSE_TIME)
		{
			// 倒れ始めたら足場の当たり判定を外し、描画だけ続ける
			for (BlockData* block : group.blocks)
			{
				block->active = false;
			}
			group.startedAt = now;
			group.state = CollapseState::Falling;
		}
		else if (group.state == CollapseState::Falling)
		{
			if (elapsed >= COLLAPSE_FALL_TIME)
			{
				group.state = CollapseState::Finished;
			}
			else
			{
				group.progress = static_cast<float>(elapsed) / COLLAPSE_FALL_TIME;
			}
		}
	}
}

void DrawMap()
{
	DrawBlock();
	const CameraData camera = GetCamera();
	for (const CollapseGroup& group : g_CollapseGroups)
	{
		if (group.state != CollapseState::Falling)
		{
			continue;
		}

		const BlockData* base = group.blocks[0];
		const float pivotX = base->pos.x + base->width * 0.5f;
		const float pivotY = base->pos.y + base->height;
		const float angle = DX_PI_F * 0.5f * group.progress * group.progress;
		const float sine = std::sin(angle);
		const float cosine = std::cos(angle);

		for (const BlockData* block : group.blocks)
		{
			const float x = block->pos.x + block->width * 0.5f - pivotX;
			const float y = block->pos.y + block->height * 0.5f - pivotY;
			const int screenX = static_cast<int>(pivotX + x * cosine - y * sine - camera.pos.x);
			const int screenY = static_cast<int>(pivotY + x * sine + y * cosine - camera.pos.y);
			DrawRotaGraph(screenX, screenY, 1.0, angle, block->handle, TRUE);
		}
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

			if (mapChipData.mapChip != NORMAL_BLOCK &&
				mapChipData.mapChip != SOIL_BLOCK &&
				mapChipData.mapChip != GRASS_BLOCK &&
				mapChipData.mapChip != ASND_BLOCK &&
				mapChipData.mapChip != ASNDNORMAL_BLOCK &&
				mapChipData.mapChip != COLLAPSE_SAND_BLOCK)
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


	// 流砂の当たり判定
	bool inQuicksand = false;
	float quicksandTop = 0.0f;

	// プレイヤーだけ流砂の処理をする
	PlayerData* player = GetPlayer();

	if (body == &player->body && player->active)
	{
		// 上昇中は沈ませない
		if (body->move.y >= 0.0f)
		{
			for (int y = 0; y < MAP_CHIP_Y_NUM; y++)
			{
				for (int x = 0; x < MAP_CHIP_X_NUM; x++)
				{
					MapChipData mapChipData = GetMapChipData(x, y);

					// 流砂以外は無視
					if (mapChipData.mapChip != QUICKSAND_BLOCK)
					{
						continue;
					}

					BlockData* block = mapChipData.data;

					if (block == nullptr || !block->active)
					{
						continue;
					}

					// プレイヤーの足元
					float playerBottom =
						body->pos.y + body->height;

					// 横方向に重なっているか
					bool hitX =
						body->pos.x < block->pos.x + block->width &&
						body->pos.x + body->width > block->pos.x;

					// 流砂の表面に到達しているか
					bool hitY =
						playerBottom + body->move.y >= block->pos.y &&
						playerBottom <= block->pos.y + block->height &&
						body->pos.y < block->pos.y;

					if (hitX && hitY)
					{
						inQuicksand = true;
						quicksandTop = block->pos.y;

						// ゆっくり沈ませる
						body->move.y = QUICKSAND_SINK_SPEED;

						// 空中扱いにする
						body->isAir = true;

						break;
					}
				}

				if (inQuicksand)
				{
					break;
				}
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

					// プレイヤーが着地した砂の山だけタイマーを開始する
					if (body == &GetPlayer()->body)
					{
						for (CollapseGroup& group : g_CollapseGroups)
						{
							if (group.state == CollapseState::Ready &&
								std::find(group.blocks.begin(), group.blocks.end(), block) != group.blocks.end())
							{
								group.startedAt = static_cast<unsigned int>(GetNowCount());
								group.state = CollapseState::Waiting;
								break;
							}
						}
					}

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


bool IsPlayerInQuicksand()
{
	PlayerData* player = GetPlayer();

	if (!player->active)
	{
		return false;
	}

	Body* body = &player->body;

	for (int y = 0; y < MAP_CHIP_Y_NUM; y++)
	{
		for (int x = 0; x < MAP_CHIP_X_NUM; x++)
		{
			MapChipData chip = GetMapChipData(x, y);

			if (chip.mapChip != QUICKSAND_BLOCK)
			{
				continue;
			}

			BlockData* block = chip.data;

			if (block == nullptr || !block->active)
			{
				continue;
			}

			// 横方向の重なり
			bool hitX =
				body->pos.x < block->pos.x + block->width &&
				body->pos.x + body->width > block->pos.x;

			// 足元が流砂に入っているか
			float playerBottom = body->pos.y + body->height;

			bool hitY = playerBottom >= block->pos.y &&
				body->pos.y < block->pos.y + block->height;

			if (hitX && hitY)
			{
				return true;
			}
		}
	}

	return false;
}

