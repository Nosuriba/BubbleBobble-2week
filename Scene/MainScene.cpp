#include "../Game.h"
#include "../ImageMng.h"
#include "../AudioMng.h"
#include "ResultScene.h"
#include "MainScene.h"
#include "../ImageMng.h"
#include "../Object/Player.h"
#include "../Object/Block.h"
#include "../HitCheck.h"

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
	player = std::make_shared<Player>(LpGame.GetScreenSize().y - 64);
	player->Init("idle",Vector2f(50,0), Vector2(48,48));
	block->Init("‚Ü‚¾‰æ‘œ‚Í‚È‚¢", Vector2f(600, 800), Vector2(0,0), Vector2(0, 0), Vector2(60, 60));


}

void MainScene::Update(const Input & p)
{
	auto hitCheck = std::make_unique<HitCheck>();

	player->CheckHit(hitCheck->HitPlayer(player->GetRect(), block->GetRect()));
	player->Update(p);
	player->Draw();
	block->Update(p);
	block->Draw();
	DxLib::DrawString(0, 0, "ƒƒCƒ“", 0x000000);
	DxLib::DrawLine(0, LpGame.GetScreenSize().y - 64,
					   LpGame.GetScreenSize().x, LpGame.GetScreenSize().y - 64,
					   0xffffff, 1.0f);
	player->Draw();
	if (p.IsTrigger(PAD_INPUT_8))
	{
		Game::GetInstance().ChangeScene(new ResultScene());
	}
}
