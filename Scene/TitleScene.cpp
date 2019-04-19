#include <DxLib.h>
#include "../Game.h"
#include "../ImageMng.h"
#include "../Object/Bubble.h"
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
	invCnt = 20;
}

void TitleScene::Update(const Input & p)
{
	invCnt--;
	if (p.IsTrigger(PAD_INPUT_8))
	{
		bubbles.clear();
		Game::GetInstance().ChangeScene(new MainScene());
		return;
	}

	/// タイトルで泡を飛ばしてみた(スタートボタンのUIなどがまだできていない)
	if (invCnt < 0)
	{
		bubbles.push_back(std::make_shared<Bubble>());
		bubbles[bubbles.size() - 1]->Init("floating", Vector2f(48 * GetRand(17), Game::GetInstance().GetScreenSize().y),
													  Vector2(48, 48));
		invCnt = 20;
	}
	for (int i = 0; i < bubbles.size(); ++i)
	{
		if (bubbles[i]->GetPos().y < -(bubbles[i]->GetRect().size.height))
		{
			bubbles.erase(bubbles.begin() + i);
			continue;
		}
		bubbles[i]->Update();
		bubbles[i]->Draw();

	}
	DxLib::DrawGraph(Game::GetInstance().GetScreenSize() .x / 7, 0, ImageMng::GetInstance().ImgGetID("resource/Image/title.png")[0], true);
}
