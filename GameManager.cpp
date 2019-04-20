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

GameManager::GameManager() :
	blockMax(28, 26), blockSize(24), charSize(48), bubbleSize(48),
	wallMax(26), wallSize(48)
{
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	stage = std::make_shared<Stage>();

	player = std::make_shared<Player>(Game::GetInstance().GetScreenSize().y - blockSize);
	player->Init("idle", Vector2f(charSize * 2, Game::GetInstance().GetScreenSize().y - (charSize + blockSize - 1)), Vector2(charSize, charSize));

	/// 敵の生成(仮設定)
	enemys.push_back(std::make_shared<Enemy>(Game::GetInstance().GetScreenSize().y - blockSize));
	enemys[0]->Init("run", Vector2f(400, Game::GetInstance().GetScreenSize().y - (charSize + blockSize - 1)), Vector2(charSize, charSize));
	CreateStage();
}

void GameManager::CreateStage()
{
	WallInstance();
	BlockInstance();
}

void GameManager::CreateBubble()
{
	if (player->ShotCheck())
	{
		bubbles.push_back(std::make_shared<Bubble>(player->GetTurnFlag()));
		bubbles[bubbles.size() - 1]->Init("shot", player->GetPos(), Vector2(bubbleSize, bubbleSize));
	}
}

void GameManager::WallInstance()
{
	for (auto i = 0; i < wallMax; i++)
	{
		auto pos = Vector2f((Game::GetInstance().GetScreenSize().x - wallSize) * (i / (wallMax / 2)),
							 wallSize + (wallSize * (i % (wallMax / 2))));

		walls.push_back(std::make_shared<Wall>());
		walls[i]->Init("resource/Image/wall.png", pos, Vector2(2, 8), Vector2(0, 0), Vector2(wallSize, wallSize));
	}
}

void GameManager::BlockInstance()
{
	int blockCnt = 0;
	int cnt = 0;
	std::string name[] = { "0" };			/// 特定のステージチップを読み込むようにしている(仮設定)
	auto blockData = stage->GetEnemyData(0, (stage->GetStageRange().Width() * stage->GetStageRange().Height()));
	auto mapSize = stage->GetStageRange();

	for (auto& tmp : blockData)
	{
		if (name[tmp] == "0")
		{
			/// ﾌﾞﾛｯｸﾃﾞｰﾀの読み込みは縦に読み込まれていくのでこのような計算になっている。
			auto pos = Vector2f(wallSize + (blockSize * (blockCnt / blockMax.y)),
								wallSize + (blockSize * (blockCnt % blockMax.y)));

			blocks.push_back(std::make_shared<Block>());
			blocks[cnt]->Init("resource/Image/block.png", pos, Vector2(2, 8), Vector2(0, 0), Vector2(blockSize, blockSize));
			cnt++;
		}
		blockCnt++;
	}
}

void GameManager::PlayerCollision()
{
	for (auto itr : walls)
	{
		if (player->HitWall(itr->GetRect()))
		{
			break;
		}
	}

	for (auto itr : blocks)
	{
		if (player->HitGround(itr->GetRect()))
		{
			break;
		}
	}
}

void GameManager::EnemyCollision()
{
	for (auto itr : enemys)
	{
		for (auto wall : walls)
		{
			if (itr->HitWall(wall->GetRect()))
			{
				break;
			}
		}

		for (auto block : blocks)
		{
			if (itr->HitGround(block->GetRect()))
			{
				break;
			}
		}
	}
}

void GameManager::BubbleCollision(const Input& p)
{
	for (auto itr = bubbles.begin(); itr != bubbles.end(); ++itr)
	{
		/// ﾌﾟﾚｲﾔｰとの当たり判定
		if ((*itr)->HitPlayer(player->GetRect(), p))
		{
			auto debug = (*itr)->HitPlayer(player->GetRect(), p);
			player->StepBubble();
		}
		
		/// 泡同士の当たり判定
		for (auto bubble = bubbles.begin(); bubble != bubbles.end(); ++bubble)
		{
			///	泡が割れるかの判定をしている
			if ((*itr)->HitBubble((*itr)->GetRect(), (*bubble)->GetRect())) 
			{
				(*itr)->ChangePop();
			}

			/// 泡同士が当たった時の移動
			auto floatState = !((*itr)->CheckFloating() && (*bubble)->CheckFloating());
			if (itr != bubble && floatState)
			{
				(*itr)->MoveContact((*bubble)->GetRect());
			}
		}

		/// 壁との当たり判定
		for (auto wall : walls)
		{
			(*itr)->HitAcross(player->GetRect(), wall->GetRect());
			if ((*itr)->HitObject((*itr)->GetRect(), wall->GetRect()))
			{
				return;
			}
		}
		/// ﾌﾞﾛｯｸとの当たり判定
		for (auto block : blocks)
		{
			if ((*itr)->HitObject((*itr)->ShotGetRect(), block->GetRect()))
			{
				return;
			}
		}
	}
}

void GameManager::Update(const Input & p)
{
	for (auto itr : walls)
	{
		itr->Draw();
	}
	for (auto itr : blocks)
	{
		itr->Draw();
	}

	PlayerCollision();
	EnemyCollision();
	BubbleCollision(p);

	player->Update(p);
	player->Draw();
	for (auto enemy : enemys)
	{
		enemy->Update();
		enemy->Draw();
	}
	

	for (int i = 0; i < bubbles.size(); ++i)
	{
		if (bubbles[i]->CheckDelete())
		{
			bubbles.erase(bubbles.begin() + i);
			continue;
		}
		bubbles[i]->Update();
		bubbles[i]->Draw();
			
	}
	CreateBubble();
	/// 泡が一定数を超えたら、最初に作られた泡を削除する
	if (bubbles.size() > 10)
	{
		bubbles[0]->ChangePop();
	}

}
