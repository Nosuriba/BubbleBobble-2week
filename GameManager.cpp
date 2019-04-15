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
	std::string name[] = { "0" };			/// ����̃X�e�[�W�`�b�v��ǂݍ��ނ悤�ɂ��Ă���(���ݒ�)
	auto chipData = stage->GetEnemyData(0, (stage->GetStageRange().Width() * stage->GetStageRange().Height()));
	auto mapSize = stage->GetStageRange();

	for (auto& tmp : chipData)
	{
		if (name[tmp] == "0")
		{
			/// ��ۯ��ް��̓ǂݍ��݂͏c�ɓǂݍ��܂�Ă����̂ł��̂悤�Ȍv�Z�ɂȂ��Ă���B
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
		if (player->HitWall(CollisionDetector::WallCollCheck(player->GetRect(), itr->GetRect()), itr->GetRect()))
		{
			break;
		}
	}

	for (auto itr : blocks)
	{
		if (player->HitGround(CollisionDetector::GroundCollCheck(player->GetRect(), itr->GetRect()), itr->GetRect()))
		{
			break;
		}
	}
}

void GameManager::BubbleCollision(const Input& p)
{
	for (auto itr : bubbles)
	{
		/// �ǂƂ̓����蔻��
		for (auto wall : walls)
		{
			if (itr->HitObject(CollisionDetector::WallCollCheck(itr->ShotGetRect(), wall->GetRect())))
			{
				return;
			}
		}
		/// ��ۯ��Ƃ̓����蔻��
		for (auto block : blocks)
		{
			if (itr->HitObject(CollisionDetector::WallCollCheck(itr->ShotGetRect(), block->GetRect())))
			{
				return;
			}
		}

		/// ��ڲ԰�Ƃ̓����蔻��
		if (itr->HitPlayer(CollisionDetector::CollCheck(itr->GetRect(), player->GetRect())))
		{
			player->StepBubble(CollisionDetector::GroundCollCheck(player->GetRect(), itr->GetRect()), p);
		}
	}
}

void GameManager::Update(const Input & p)
{
	player->Update(p);
	CreateBubble();
	PlayerCollision();		/// �v���C���[�̓����蔻������o���Ă���
	BubbleCollision(p);

	player->Draw();

	for (auto itr : walls)
	{
		itr->Draw();
	}
	for (auto itr : blocks)
	{
		itr->Draw();
	}
	for (auto itr : bubbles)
	{
		itr->Update();
		itr->Draw();
		itr->DeleteBubble(itr);
	}
	
}
