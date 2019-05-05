#include <DxLib.h>
#include "../Game.h"
#include "../GameManager.h"
#include "MainScene.h"
#include "ResultScene.h"

MainScene::MainScene()
{
	Init();
}

MainScene::~MainScene()
{
}

void MainScene::Init()
{
	game = std::make_shared<GameManager>();
	game->Init();
}

void MainScene::Update(const Input & p)
{	
	game->Update(p);
	if (p.IsTrigger(PAD_INPUT_10))
	{
		Game::GetInstance().ChangeScene(new ResultScene());
		return;
	}

}
