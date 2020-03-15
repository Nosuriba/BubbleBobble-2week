#include <DxLib.h>
#include "../Game.h"
#include "../ImageMng.h"
#include "../Object/Bubble.h"
#include "TitleScene.h"
#include "MainScene.h"

TitleScene::TitleScene()
{
	Init();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	pos	   = Vector2f(Game::GetInstance().GetScreenSize().x / 7, -423.0f);
	vel.y  = 5.f;
	invCnt = 30;
	dispCnt = 0;
}

void TitleScene::Update(const Input & p)
{
	if ((int)pos.y >= 0)
	{
		invCnt--;
		dispCnt++;
		if (p.IsTrigger(PAD_INPUT_10))
		{
			bubbles.clear();
			Game::GetInstance().ChangeScene(new MainScene());
			return;
		}

		/// ƒ^ƒCƒgƒ‹‚Å–A‚ð”ò‚Î‚µ‚Ä‚Ý‚½
		if (invCnt < 0)
		{
			bubbles.push_back(std::make_shared<Bubble>());
			bubbles[bubbles.size() - 1]->Init("floating", Vector2f(48 * GetRand(17), Game::GetInstance().GetScreenSize().y),
				Vector2(48, 48));
			invCnt = 30;
		}
		for (unsigned int i = 0; i < bubbles.size(); ++i)
		{
			if (bubbles[i]->GetPos().y < -(bubbles[i]->GetRect().size.height))
			{
				bubbles.erase(bubbles.begin() + i);
				continue;
			}
			bubbles[i]->Update();
			bubbles[i]->Draw();

		}

		if ((dispCnt / 30) % 2)
		DrawExtendString(210, 500, 2.0, 2.0, "PRESS SPACE START", 0xffffff);
	}
	else
	{
		pos += vel;
		if (p.IsTrigger(PAD_INPUT_10))
		{
			pos.y = 0;
		}
	}
	DxLib::DrawGraph(pos.x, pos.y, ImageMng::GetInstance().ImgGetID("resource/Image/title.png"), true);
}
