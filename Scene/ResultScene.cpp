#include <DxLib.h>
#include "../Game.h"
#include "../ImageMng.h"
#include "ResultScene.h"
#include "TitleScene.h"

ResultScene::ResultScene()
{
}


ResultScene::~ResultScene()
{
}

void ResultScene::Init()
{

}

void ResultScene::Update(const Input & p)
{
	if (p.IsTrigger(PAD_INPUT_8))
	{
		Game::GetInstance().ChangeScene(new TitleScene());
	}

	DxLib::DrawString(0, 0, "ƒŠƒUƒ‹ƒg", 0x000000);
	DxLib::DrawGraph(0, 0, ImageMng::GetInstance().ImgGetID(ImageMng::GetInstance().GetImage().resultImage)[0], true);
}
