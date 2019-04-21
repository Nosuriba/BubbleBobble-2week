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

	/// ìGÇÃê∂ê¨(âºê›íË)
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
	std::string name[] = { "0" };			/// ì¡íËÇÃÉXÉeÅ[ÉWÉ`ÉbÉvÇì«Ç›çûÇﬁÇÊÇ§Ç…ÇµÇƒÇ¢ÇÈ(âºê›íË)
	auto blockData = stage->GetEnemyData(0, (stage->GetStageRange().Width() * stage->GetStageRange().Height()));
	auto mapSize = stage->GetStageRange();

	for (auto& tmp : blockData)
	{
		if (name[tmp] == "0")
		{
			/// Ãﬁ€Ø∏√ﬁ∞¿ÇÃì«Ç›çûÇ›ÇÕècÇ…ì«Ç›çûÇ‹ÇÍÇƒÇ¢Ç≠ÇÃÇ≈Ç±ÇÃÇÊÇ§Ç»åvéZÇ…Ç»Ç¡ÇƒÇ¢ÇÈÅB
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

void GameManager::EnemyCollision(const Input& p)
{
	for (auto itr : enemys)
	{
		for (auto wall : walls)
		{
			if (itr->HitPlayer(player->GetRect(), wall->GetRect(), p))
			{
				player->StepBubble();
			}

			if (itr->HitWall(wall->GetRect()))
			{
				break;
			}
			
			itr->DieControl(wall->GetRect());
		}

		for (auto block : blocks)
		{
			/*if (itr->UpperCheck(player->GetRect(), block->GetRect()))
			{
				break;
			}*/
			if (itr->HitGround(block->GetRect()))
			{
				break;
			}
			itr->DieControl(block->GetRect());
		}

		for (auto bubble = bubbles.begin(); bubble != bubbles.end(); bubble++)
		{
			if (itr->HitBubble((*bubble)->GetRect()))
			{
				bubbles.erase(bubble);
				break;
			}
		}

		
	}
}

void GameManager::BubbleCollision(const Input& p)
{
	for (auto itr = bubbles.begin(); itr != bubbles.end(); ++itr)
	{
		/// Ãﬂ⁄≤‘∞Ç∆ÇÃìñÇΩÇËîªíË
		if ((*itr)->HitPlayer(player->GetRect(), p))
		{
			auto debug = (*itr)->HitPlayer(player->GetRect(), p);
			player->StepBubble();
		}
		
		/// ñAìØémÇÃìñÇΩÇËîªíË
		for (auto bubble = bubbles.begin(); bubble != bubbles.end(); ++bubble)
		{
			///	ñAÇ™äÑÇÍÇÈÇ©ÇÃîªíËÇÇµÇƒÇ¢ÇÈ
			if ((*itr)->HitBubble((*bubble)->GetRect())) 
			{
				(*bubble)->ChangePop();
			}

			if (itr != bubble)
			{
				(*bubble)->MoveContact((*itr)->GetRect());
			}
		}

		/// ï«Ç∆ÇÃìñÇΩÇËîªíË
		for (auto wall : walls)
		{
			(*itr)->HitAcross(player->GetRect(), wall->GetRect());
			if ((*itr)->HitObject(wall->GetRect()))
			{
				return;
			}
		}
		/// Ãﬁ€Ø∏Ç∆ÇÃìñÇΩÇËîªíË
		for (auto block : blocks)
		{
			if ((*itr)->HitObject(block->GetRect()))
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
	EnemyCollision(p);
	BubbleCollision(p);

	player->Update(p);
	player->Draw();
	for (auto enemy : enemys)
	{
		enemy->Update();
		enemy->Draw();
	}
	

	for (unsigned int i = 0; i < bubbles.size(); ++i)
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
	/// ñAÇ™àÍíËêîÇí¥Ç¶ÇΩÇÁÅAç≈èâÇ…çÏÇÁÇÍÇΩñAÇçÌèúÇ∑ÇÈ
	if (bubbles.size() > 10)
	{
		bubbles[0]->ChangePop();
	}

}
