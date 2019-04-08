#include "../Game.h"
#include "../ImageMng.h"
#include "../AudioMng.h"
#include "ResultScene.h"
#include "MainScene.h"
#include "../ImageMng.h"
#include "../Object/Player.h"

MainScene::MainScene()
{
	Init();
}


MainScene::~MainScene()
{
}

void MainScene::Init()
{
	player = std::make_shared<Player>(Game::GetInstance().GetScreenSize().y - 64);
	player->Init(LpImageMng.GetImage().playerImage, "idle",
				 Vector2f(50,0), Vector2(50, 4), Vector2(20, 20));


}

void MainScene::Update(const Input & p)
{
	player->Update(p);
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
