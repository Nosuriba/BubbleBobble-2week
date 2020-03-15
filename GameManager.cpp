#include "GameManager.h"
#include "Input.h"
#include "Game.h"
#include "Stage.h"
#include "AudioMng.h"
#include "Object/Player.h"
#include "Object/Enemy.h"
#include "Object/Bubble.h"
#include "StageObject/Block.h"
#include "StageObject/Wall.h"

const Vector2 blockMax = Vector2(28,26);
const int blockSize	   = 24;
const int charSize	   = 48;
const int bubbleSize   = 48;
const int wallSize     = 48;
const int wallMax	   = 26;

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	/// ステージの生成
	stage = std::make_shared<Stage>();

	/// プレイヤーの生成
	player = std::make_shared<Player>(Game::GetInstance().GetScreenSize().y - blockSize);
	player->Init("idle", Vector2f(charSize * 2, Game::GetInstance().GetScreenSize().y - (charSize + blockSize - 1)), Vector2(charSize, charSize));

	/// 敵の生成
	for (int i = 0; i < 3; ++i)
	{
		enemies.push_back(std::make_shared<Enemy>(Game::GetInstance().GetScreenSize().y - blockSize));
		enemies[i]->Init("run", Vector2f(Game::GetInstance().GetScreenSize().x / 2 - blockSize, (i * charSize) - (charSize * 3)),
								Vector2(charSize, charSize));
	}
	CreateStage();
}

void GameManager::CreateStage()
{
	/// ゲーム中のオブジェクト生成
	WallInstance();
	BlockInstance();
}

void GameManager::CreateBubble()
{
	if (player->IsShot())
	{
		bubbles.push_back(std::make_shared<Bubble>(player->GetTurnFlag()));
		bubbles[bubbles.size() - 1]->Init("shot", player->GetPos(), Vector2(bubbleSize, bubbleSize));
	}
}

void GameManager::WallInstance()
{
	for (auto i = 0; i < wallMax; i++)
	{
		/// 壁の座標初期化
		auto pos = Vector2f((Game::GetInstance().GetScreenSize().x - wallSize) * (i / (wallMax / 2)),
							 wallSize + (wallSize * (i % (wallMax / 2))));

		/// 壁の生成
		walls.push_back(std::make_shared<Wall>());
		walls[i]->Init("resource/Image/wall.png", pos, Vector2(2, 8), Vector2(0, 0), Vector2(wallSize, wallSize));
	}
}

void GameManager::BlockInstance()
{
	int blockCnt = 0;
	int cnt		 = 0;							/// ブロックの番地アクセス用
	std::string name[] = { "0", "2" };			/// 特定のステージチップを読み込むようにしている(仮設定)
	auto blockData = stage->GetEnemyData(0, (stage->GetStageRange().Width() * stage->GetStageRange().Height()));
	auto mapSize = stage->GetStageRange();

	for (auto& tmp : blockData)
	{
		if (name[tmp] == "0")
		{
			/// ブロックデータの座標読み込み
			auto pos = Vector2f(wallSize + (blockSize * (blockCnt / blockMax.y)),
								wallSize + (blockSize * (blockCnt % blockMax.y)));

			/// ブロックの生成
			blocks.push_back(std::make_shared<Block>());
			blocks[cnt]->Init("resource/Image/block.png", pos, Vector2(2, 8), Vector2(0, 0), Vector2(blockSize, blockSize));
			cnt++;
		}
		blockCnt++;
	}
}

void GameManager::PlayerCollision()
{
	/// プレイヤーの当たり判定
	for (auto itr : walls)
	{
		/// 壁との当たり判定
		if (player->IsHitWall(itr->GetRect()))
		{
			break;
		}
	}

	for (auto itr : blocks)
	{
		/// ブロックとの当たり判定
		if (player->isGround(itr->GetRect()))
		{
			break;
		}
	}

	for (auto itr : enemies)
	{
		/// 敵との当たり判定
		if (player->IsHitEnemy(itr->GetRect(), itr->IsAlive()))
		{
			break;
		}
	}
}

void GameManager::EnemyCollision(const Input& p)
{
	//// 敵の当たり判定
	for (auto itr : enemies)
	{
		for (auto wall : walls)
		{
			/// プレイヤーとの当たり判定
			if (itr->HitPlayer(player->GetRect(), wall->GetRect(), p))
			{
				player->IsStepBubble();
			}

			/// 壁との当たり判定
			if (itr->HitWall(wall->GetRect()))
			{
				break;
			}
			itr->DieControl(wall->GetRect());
		}

		for (auto block : blocks)
		{
			/// プレイヤーの上方向との当たり判定
			if (itr->UpperCheck(player->GetRect(), block->GetRect()))
			{
				break;
			}

			/// 床との当たり判定
			if (itr->HitGround(player->GetRect(), block->GetRect()))
			{
				break;
			}
			itr->DieControl(block->GetRect());
		}

		for (auto bubble = bubbles.begin(); bubble != bubbles.end(); bubble++)
		{
			/// 泡との当たり判定
			if (itr->HitBubble((*bubble)->GetRect(), (*bubble)->HitEnemy(itr->GetRect())))
			{
				bubbles.erase(bubble);
				break;
			}
		}
	}
}

void GameManager::BubbleCollision(const Input& p)
{
	//// 泡の当たり判定
	for (auto itr = bubbles.begin(); itr != bubbles.end(); ++itr)
	{
		/// プレイヤーとの当たり判定
		if ((*itr)->HitPlayer(player->GetRect(), p))
		{
			auto debug = (*itr)->HitPlayer(player->GetRect(), p);
			player->IsStepBubble();
		}
		
		for (auto bubble = bubbles.begin(); bubble != bubbles.end(); ++bubble)
		{
			///	泡が割れるかの判定
			if ((*itr)->HitBubble((*bubble)->GetRect())) 
			{
				(*bubble)->ChangePop();
			}

			/// 浮いている泡同士が当たった時の判定
			if (itr != bubble)
			{
				/// 泡同士が接触した時、移動する
				(*itr)->MoveContact((*bubble)->GetRect());
			}
		}

		for (auto wall : walls)
		{
			/// 壁との当たり判定
			(*itr)->HitAcross(player->GetRect(), wall->GetRect());
			if ((*itr)->HitObject(wall->GetRect()))
			{
				return;
			}
		}

		for (auto block : blocks)
		{
			/// ブロックとの当たり判定
			if ((*itr)->HitObject(block->GetRect()))
			{
				return;
			}
		}
	}
}

void GameManager::Update(const Input & p)
{
	/// オブジェクトの描画
	for (auto itr : walls)
	{
		itr->Draw();
	}
	for (auto itr : blocks)
	{
		itr->Draw();
	}

	/// キャラクターの当たり判定
	PlayerCollision();
	EnemyCollision(p);

	/// 泡の当たり判定
	BubbleCollision(p);

	/// プレイヤーの更新
	player->Update(p);
	player->Draw();

	/// 敵の更新
	for (auto enemy : enemies)
	{
		enemy->Update();
		enemy->Draw();
	}

	/// 泡の更新
	for (unsigned int i = 0; i < bubbles.size(); ++i)
	{
		if (bubbles[i]->IsDelete())
		{
			bubbles.erase(bubbles.begin() + i);
			continue;
		}
		bubbles[i]->Update();
		bubbles[i]->Draw();
			
	}

	/// 泡の生成
	CreateBubble();

	/// 泡が一定数を超えたら、最初に作られた泡を削除する
	if (bubbles.size() > 10)
	{
		bubbles[0]->ChangePop();
	}

}
