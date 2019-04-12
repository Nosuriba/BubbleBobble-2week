#include <list>		// デバッグ用で使うために追加している。

#include "MainScene.h"
#include "ResultScene.h"
#include "../Stage.h"
#include "../Game.h"
#include "../ImageMng.h"
#include "../AudioMng.h"
#include "../ImageMng.h"
#include "../Object/Player.h"
#include "../StageObject/Block.h"
#include "../StageObject/Wall.h"
#include "../CollisionDetector.h"

MainScene::MainScene() : 
	blockMax(28,26), blockSize(24), playerSize(48),
	wallMax(26), wallSize(48)

{
	Init();
}


MainScene::~MainScene()
{
}

void MainScene::Init()
{
	int blockCnt = 0;
	int cnt = 0;
	std::string name[] = {"0"};			/// 特定のステージチップを読み込むようにしている(仮設定)
	stage = std::make_shared<Stage>();

	auto wallPos = Vector2f(wallSize, wallSize);
	auto chipData = stage->GetEnemyData(0, (stage->GetStageRange().Width() * stage->GetStageRange().Height()));
	auto mapSize = stage->GetStageRange();

	for (auto& tmp : chipData)
	{
		if (name[tmp] == "0")
		{
			/// ﾌﾞﾛｯｸﾃﾞｰﾀの読み込みは縦に読み込まれていくからこのような計算になっている。
			auto pos = Vector2f(wallPos.x + (blockSize * (blockCnt / blockMax.y)), 
								wallPos.y + (blockSize * (blockCnt % blockMax.y)));
			
			blocks.push_back(std::make_shared<Block>());
			blocks[cnt]->Init(ImageMng::GetInstance().GetImage().chipImage, pos, Vector2(2, 24), Vector2(0,0), Vector2(blockSize, blockSize));
			cnt++;
		}
		blockCnt++;
	}

	/// 後にゲームマネージャーのクラスに、持って行ったほうがいいかもしれない処理共だよ
	for (auto i = 0; i < wallMax; i++)
	{
		auto pos = Vector2f((Game::GetInstance().GetScreenSize().x - wallSize) * (i / (wallMax / 2)), wallSize + (wallSize * (i % (wallMax / 2))));
		wall.push_back(std::make_shared<Wall>());
		wall[i]->Init(ImageMng::GetInstance().GetImage().wallImage, pos, Vector2(2, 24), Vector2(0, 0), Vector2(wallSize, wallSize));
	}
	player = std::make_shared<Player>(Game::GetInstance().GetScreenSize().y - blockSize);
	player->Init("idle",Vector2f(playerSize, Game::GetInstance().GetScreenSize().y + wallSize), Vector2(playerSize, playerSize));
	}

void MainScene::Update(const Input & p)
{
	player->Update(p);
	player->Draw();

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

	/// 壁の描画と壁に当たったかの処理
	for (auto itr : wall)
	{
		itr->Draw();
		itr->Update();
	}
	for (auto itr : wall)
	{
		if (player->HitWall(CollisionDetector::WallCollCheck(player->GetRect(), itr->GetRect()), itr->GetRect()))
		{
			break;
		}
	}

	player->Draw();
	if (p.IsTrigger(PAD_INPUT_8))
	{
		Game::GetInstance().ChangeScene(new ResultScene());
	}
}
