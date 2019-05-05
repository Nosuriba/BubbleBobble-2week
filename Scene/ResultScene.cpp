#include <DxLib.h>
#include "../Game.h"
#include "../ImageMng.h"
#include "../AudioMng.h"
#include "ResultScene.h"
#include "TitleScene.h"

ResultScene::ResultScene()
{
	Init();
}


ResultScene::~ResultScene()
{
}

void ResultScene::Init()
{
	dispCnt = 0;
}

void ResultScene::Update(const Input & p)
{
	dispCnt++;
	if (p.IsTrigger(PAD_INPUT_10))
	{
		LpAudioMng.StopBGM();
		Game::GetInstance().ChangeScene(new TitleScene());
		return;
	}
	LpAudioMng.PlayBGM(LpAudioMng.GetSound().resultBGM);
	DxLib::DrawString(0, 0, "ƒŠƒUƒ‹ƒg", 0x000000);
	DxLib::DrawGraph(50, 0, ImageMng::GetInstance().ImgGetID("resource/Image/result.png")[0], true);

	if ((dispCnt / 30) % 2)
		DrawExtendString(210, 500, 2.0, 2.0, "PRESS SPACE TITLE", 0xffffff);
}
