#include <DxLib.h>
#include "../Game.h"
#include "../ImageMng.h"
#include "../AudioMng.h"
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
		LpAudioMng.StopBGM();
		Game::GetInstance().ChangeScene(new TitleScene());
		return;
	}
	LpAudioMng.PlayBGM(LpAudioMng.GetSound().resultBGM);
	DxLib::DrawString(0, 0, "ƒŠƒUƒ‹ƒg", 0x000000);
	DxLib::DrawGraph(50, 0, ImageMng::GetInstance().ImgGetID("resource/Image/result.png")[0], true);
}
