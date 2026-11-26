
#include "MapManager.h"
#include "MapParameter.h"
#include "MapChip.h"
#include "Block.h"
#include "../Collision/Collision.h"
#include "../Player/Player.h"

// キャラクターの周囲何マスまでチェックするか
#define CHECK_ROUND_NUM (2)

// 流砂の沈む速さ
#define QUICKSAND_SINK_SPEED (0.8f)

 // 崩れる砂ブロックの時間（ミリ秒）
#define COLLAPSE_TIME (1000)

// 崩れる砂ブロックのタイマー
bool g_CollapseStarted[BLOCK_MAX] = { false };
int g_CollapseStartTime[BLOCK_MAX] = { 0 };



void InitMap()
{
	InitBlock();

	// 崩れる砂ブロックの初期化
	for (int i = 0; i < BLOCK_MAX; i++)
	{
		g_CollapseStarted[i] = false;
		g_CollapseStartTime[i] = 0;
	}
}


void LoadMap()
{
	LoadMapChipData();
	LoadBlock();
}

void StartMap()
{
	CreateMap();
}


void UpdateMap()
{
	// プレイヤー取得
	PlayerData* player = GetPlayer();

	// ブロック取得
	BlockData* block = GetBlocks();

	for (int i = 0; i < BLOCK_MAX; i++, block++)
	{
		// 使用していないブロックは無視
		if (!block->active)
		{
			continue;
		}

		// 崩れる砂ブロック以外は無視
		if (block->type != ASNDTRIANGLE_BLOCK)
		{
			continue;
		}

		// タイマーが始まっていない場合
		if (!g_CollapseStarted[i])
		{
			// 死亡しているプレイヤーでは発動させない
			if (!player->active)
			{
				continue;
			}

			// プレイヤーの足元
			float playerBottom = player->body.pos.y + player->body.height;

			// 横方向の重なり
			bool hitX =
				player->body.pos.x < block->pos.x + block->width &&
				player->body.pos.x + player->body.width > block->pos.x;

			// ブロックの上に乗っているか
			bool hitY =
				playerBottom >= block->pos.y - 2.0f &&
				playerBottom <= block->pos.y + 3.0f;

			if (hitX && hitY)
			{
				// タイマー開始
				g_CollapseStarted[i] = true;
				g_CollapseStartTime[i] = GetNowCount();
			}
		}
		else
		{
			// 経過時間
			int elapsed = GetNowCount() - g_CollapseStartTime[i];

			// 1秒経過したら消える
			if (elapsed >= COLLAPSE_TIME)
			{
				block->active = false;
			}
		}
	}
}


void DrawMap()
{
	DrawBlock();
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

