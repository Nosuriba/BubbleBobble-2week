#include "../Game.h"
#include "../ImageMng.h"
#include "../AudioMng.h"
#include "ResultScene.h"
#include "MainScene.h"
#include "../ImageMng.h"
#include "../Object/Player.h"
#include "../Object/Block.h"
#include "../CollisionDetector.h"

MainScene::MainScene()
{
	Init();
}


MainScene::~MainScene()
{
}

void MainScene::Init()
{
	block = std::make_shared<Block>();
	player = std::make_shared<Player>(LpGame.GetScreenSize().y - 48);
	player->Init("idle",Vector2f(50, LpGame.GetScreenSize().y + 48), Vector2(48,48));
	block->Init("‚Ü‚¾‰æ‘œ‚Í‚È‚¢", Vector2f(500,500), Vector2(0,0), Vector2(0, 0), Vector2(24,24));


}

void MainScene::Update(const Input & p)
{
	// player->HitWall(CollisionDetector::WallCollCheck(player->GetRect(), block->GetRect()));
	player->HitGround(CollisionDetector::GroundCollCheck(player->GetRect(), block->GetRect()), block->GetRect());
	player->Update(p);
	player->Draw();
	block->Update(p);
	block->Draw();
	DxLib::DrawString(0, 0, "ƒƒCƒ“", 0x000000);
	DxLib::DrawLine(0, LpGame.GetScreenSize().y - 48,
					   LpGame.GetScreenSize().x, LpGame.GetScreenSize().y - 48,
					   0xffffff, 1.0f);
	player->Draw();
	if (p.IsTrigger(PAD_INPUT_8))
	{
		Game::GetInstance().ChangeScene(new ResultScene());
	}
}
