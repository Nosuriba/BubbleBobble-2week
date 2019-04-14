#include "GameManager.h"
#include "Input.h"
#include "Game.h"
#include "Stage.h"
#include "ImageMng.h"
#include "AudioMng.h"
#include "Object/Player.h"
#include "StageObject/Block.h"
#include "StageObject/Wall.h"
#include "CollisionDetector.h"

GameManager::GameManager() :
	blockMax(28, 26), blockSize(24), playerSize(48),
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

	CreateBlock();
}

void GameManager::CreateBlock()
{
	int blockCnt = 0;
	int cnt		 = 0;
	std::string name[] = { "0" };			/// 特定のステージチップを読み込むようにしている(仮設定)
	auto chipData	   = stage->GetEnemyData(0, (stage->GetStageRange().Width() * stage->GetStageRange().Height()));
	auto mapSize	   = stage->GetStageRange();

	/// 壁の生成
	for (auto i = 0; i < wallMax; i++)
	{
		auto pos = Vector2f((Game::GetInstance().GetScreenSize().x - wallSize) * (i / (wallMax / 2)),
			wallSize + (wallSize * (i % (wallMax / 2))));

		walls.push_back(std::make_shared<Wall>());
		walls[i]->Init(ImageMng::GetInstance().GetImage().wallImage, pos, Vector2(2, 24), Vector2(0, 0), Vector2(wallSize, wallSize));
	}

	/// ﾌﾞﾛｯｸの生成
	for (auto& tmp : chipData)
	{
		if (name[tmp] == "0")
		{
			/// ﾌﾞﾛｯｸﾃﾞｰﾀの読み込みは縦に読み込まれていくからこのような計算になっている。
			auto pos = Vector2f(wallSize + (blockSize * (blockCnt / blockMax.y)),
								wallSize + (blockSize * (blockCnt % blockMax.y)));

			blocks.push_back(std::make_shared<Block>());
			blocks[cnt]->Init(ImageMng::GetInstance().GetImage().chipImage, pos, Vector2(2, 24), Vector2(0, 0), Vector2(blockSize, blockSize));
			cnt++;
		}
		blockCnt++;
	}

	
}

void GameManager::Update(const Input & p)
{
	player->Update(p);
	player->Draw();

	/// 壁の描画と壁に当たったかの処理
	for (auto itr : walls)
	{
		itr->Draw();
		itr->Update();
	}
	for (auto itr : walls)
	{
		if (player->HitWall(CollisionDetector::WallCollCheck(player->GetRect(), itr->GetRect()), itr->GetRect()))
		{
			break;
		}
	}

	/// ﾌﾞﾛｯｸの描画とﾌﾞﾛｯｸの床に当たったかの処理
	for (auto itr : blocks)
	{
		itr->Draw();
	}
	for (auto itr : blocks)
	{
		if (player->HitGround(CollisionDetector::GroundCollCheck(player->GetRect(), itr->GetRect()), itr->GetRect()))
		{
			break;
		}
	}

	
}
