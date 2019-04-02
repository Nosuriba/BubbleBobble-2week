#include "../Game.h"
#include "../ImageMng.h"
#include "../AudioMng.h"
#include "ResultScene.h"
#include "MainScene.h"

MainScene::MainScene()
{
}


MainScene::~MainScene()
{
}

void MainScene::Init()
{

}

void MainScene::Update(const Input & p)
{
	if (p.IsTrigger(PAD_INPUT_8))
	{
		Game::GetInstance().ChangeScene(new ResultScene());
	}

	DxLib::DrawString(0, 0, "ÉÅÉCÉì", 0x000000);
	DxLib::DrawGraph(0, 0, LpImageMng.ImgGetID(LpImageMng.GetImage().mainImage)[0], true);
	
}
