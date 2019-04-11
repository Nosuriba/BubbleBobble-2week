#include "../Game.h"
#include "../ImageMng.h"
#include "../AudioMng.h"
#include "ResultScene.h"
#include "MainScene.h"
#include "../ImageMng.h"
#include "../Object/Player.h"
#include "../StageObject/Block.h"
#include "../StageObject/Wall.h"
#include "../CollisionDetector.h"

MainScene::MainScene() : wallCnt(26)
{
	Init();
}


MainScene::~MainScene()
{
}

void MainScene::Init()
{
	/// 後にゲームマネージャーのクラスに、持って行ったほうがいいかもしれない処理共だよ
	/// マジックナンバーは後で修正するようにする
	for (auto i = 0; i < wallCnt; i++)
	{
		auto pos = Vector2f((Game::GetInstance().GetScreenSize().x - 48) * (i / (wallCnt / 2)), 96 + (48 * (i % (wallCnt / 2))));
		wall.push_back(std::make_shared<Wall>());
		wall[i]->Init(ImageMng::GetInstance().GetImage().wallImage, pos, Vector2(2, 24), Vector2(0, 0), Vector2(48, 48));
	}
	block = std::make_shared<Block>();
	player = std::make_shared<Player>(Game::GetInstance().GetScreenSize().y - 48);
	player->Init("idle",Vector2f(50, Game::GetInstance().GetScreenSize().y + 48), Vector2(48,48));
	block->Init(ImageMng::GetInstance().GetImage().chipImage, Vector2f(500, 640), Vector2(2,24), Vector2(0, 0), Vector2(24, 24));
}

void MainScene::Update(const Input & p)
{
	player->Update(p);
	player->Draw();
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
	player->HitGround(CollisionDetector::GroundCollCheck(player->GetRect(), block->GetRect()), block->GetRect());
	
	block->Update(p);
	block->Draw();
	DxLib::DrawString(0, 0, "メイン", 0x000000);
	DxLib::DrawLine(0, Game::GetInstance().GetScreenSize().y - 48,
					   Game::GetInstance().GetScreenSize().x, Game::GetInstance().GetScreenSize().y - 48,
					   0xffffff, 1.0f);
	player->Draw();
	if (p.IsTrigger(PAD_INPUT_8))
	{
		Game::GetInstance().ChangeScene(new ResultScene());
	}
}
