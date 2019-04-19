#include "GameManager.h"
#include "Input.h"
#include "Game.h"
#include "Stage.h"
#include "AudioMng.h"
#include "CollisionDetector.h"
#include "Object/Player.h"
#include "Object/Bubble.h"
#include "StageObject/Block.h"
#include "StageObject/Wall.h"

GameManager::GameManager() :
	blockMax(28, 26), blockSize(24), playerSize(48), bubbleSize(48),
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
	player->Init("idle", Vector2f(playerSize * 2, Game::GetInstance().GetScreenSize().y - blockSize), Vector2(playerSize, playerSize));

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
		walls[i]->Init("resource/Image/wall.png", pos, Vector2(2, 24), Vector2(0, 0), Vector2(wallSize, wallSize));
	}
}

void GameManager::BlockInstance()
{
	int blockCnt = 0;
	int cnt = 0;
	std::string name[] = { "0" };			/// ì¡íËÇÃÉXÉeÅ[ÉWÉ`ÉbÉvÇì«Ç›çûÇﬁÇÊÇ§Ç…ÇµÇƒÇ¢ÇÈ(âºê›íË)
	auto chipData = stage->GetEnemyData(0, (stage->GetStageRange().Width() * stage->GetStageRange().Height()));
	auto mapSize = stage->GetStageRange();

	for (auto& tmp : chipData)
	{
		if (name[tmp] == "0")
		{
			/// Ãﬁ€Ø∏√ﬁ∞¿ÇÃì«Ç›çûÇ›ÇÕècÇ…ì«Ç›çûÇ‹ÇÍÇƒÇ¢Ç≠ÇÃÇ≈Ç±ÇÃÇÊÇ§Ç»åvéZÇ…Ç»Ç¡ÇƒÇ¢ÇÈÅB
			auto pos = Vector2f(wallSize + (blockSize * (blockCnt / blockMax.y)),
								wallSize + (blockSize * (blockCnt % blockMax.y)));

			blocks.push_back(std::make_shared<Block>());
			blocks[cnt]->Init("resource/Image/block.png", pos, Vector2(2, 24), Vector2(0, 0), Vector2(blockSize, blockSize));
			cnt++;
		}
		blockCnt++;
	}
}

void GameManager::PlayerCollision()
{
	for (auto itr : walls)
	{
		if (player->HitWall(CollisionDetector::SideCollCheck(player->GetRect(), itr->GetRect()), itr->GetRect()))
		{
			break;
		}
	}

	for (auto itr : blocks)
	{
		if (player->HitGround(CollisionDetector::UnderCollCheck(player->GetRect(), itr->GetRect()), itr->GetRect()))
		{
			break;
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
			player->StepBubble();
		}
		
		/// ñAìØémÇÃìñÇΩÇËîªíË
		for (auto bubble = bubbles.begin(); bubble != bubbles.end(); ++bubble)
		{
			///	ñAÇ™äÑÇÍÇÈÇ©ÇÃîªíËÇÇµÇƒÇ¢ÇÈ
			if ((*itr)->HitBubble((*itr)->GetRect(), (*bubble)->GetRect())) 
			{
				(*itr)->ChangePop();
			}

			/// ñAìØémÇ™ìñÇΩÇ¡ÇΩéûÇÃà⁄ìÆ
			auto floatState = !((*itr)->CheckFloating() && (*bubble)->CheckFloating());
			if (itr != bubble && floatState)
			{
				(*itr)->MoveContact((*bubble)->GetRect());
			}
		}

		/// ï«Ç∆ÇÃìñÇΩÇËîªíË
		for (auto wall : walls)
		{
			(*itr)->HitAcross(player->GetRect(), wall->GetRect());
			if ((*itr)->HitObject((*itr)->GetRect(), wall->GetRect()))
			{
				return;
			}
		}
		/// Ãﬁ€Ø∏Ç∆ÇÃìñÇΩÇËîªíË
		for (auto block : blocks)
		{
			if ((*itr)->HitObject((*itr)->ShotGetRect(), block->GetRect()))
			{
				return;
			}
		}
		for (auto wall : walls)
		{
			
		}
	}
}

void GameManager::Update(const Input & p)
{
	player->Update(p);
	player->Draw();
	PlayerCollision();		/// ÉvÉåÉCÉÑÅ[ÇÃìñÇΩÇËîªíËÇåüèoÇµÇƒÇ¢ÇÈ
	BubbleCollision(p);

	for (auto itr : walls)
	{
		itr->Draw();
	}
	for (auto itr : blocks)
	{
		itr->Draw();
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
	/// ñAÇ™àÍíËêîÇí¥Ç¶ÇΩÇÁÅAç≈èâÇ…çÏÇÁÇÍÇΩñAÇçÌèúÇ∑ÇÈ
	if (bubbles.size() > 10)
	{
		bubbles[0]->ChangePop();
	}

}
