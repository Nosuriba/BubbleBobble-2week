#include <DxLib.h>
#include "../Game.h"
#include "../ImageMng.h"
#include "TitleScene.h"
#include "MainScene.h"

TitleScene::TitleScene()
{
}


TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
}

void TitleScene::Update(const Input & p)
{
	if (p.IsTrigger(PAD_INPUT_8))
	{
		Game::GetInstance().ChangeScene(new MainScene());
	}
	
	DxLib::DrawString(0, 0, "ƒ^ƒCƒgƒ‹", 0x000000);
	DxLib::DrawGraph(0, 0, ImageMng::GetInstance().ImgGetID(ImageMng::GetInstance().GetImage().titleImage)[0], true);
}
