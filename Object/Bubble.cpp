#include <DxLib.h>
#include "Bubble.h"

Bubble::Bubble(const bool& bubbleDir) : spitFrame(10)
{
	///	‰¼‚Ì‰Šú‰»
	Shot();
	nowCutIdx = 0;
	ReadActionFile("Action/bubble.act");		// –¼‘O‚Í‚±‚¤‚·‚é—\’è
	bubbleImage = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	this->bubbleDir = bubbleDir;
	deleteFlag = false;
	pos		= vel = Vector2f(0, 0);
	size	= Vector2();
	invCnt  = spitFrame;
}

Bubble::~Bubble()
{
	DxLib::DeleteGraph(bubbleImage);
}

Rect Bubble::GetRect()
{
	auto center = Vector2(pos.x + (size.x / 2), pos.y + (size.x / 2));
	auto rectSize = Size(size.x, size.y);

	return Rect(center, rectSize);
}

Rect Bubble::ShotGetRect()
{
	auto center = Vector2(pos.x + (size.x / 2), pos.y + (size.x / 2));
	auto rectSize = Size(size.x / 2, size.y / 2);

	return Rect(center, rectSize);
}

const bool& Bubble::CheckDelete()
{
	return deleteFlag;
}

bool Bubble::CheckDebugF()
{
	return (updater == &Bubble::FloatingUpdate);
}

const bool& Bubble::HitPlayer(const bool& hitFlag)
{
	if (updater == &Bubble::FloatingUpdate && hitFlag)
	{
		Pop();
		return true;
	}
	return false;
}

const bool& Bubble::HitEnemy(const bool &hitFlag)
{
	if (updater == &Bubble::ShotUpdate && hitFlag)
	{
		deleteFlag = false;
		return true;
	}
	return false;
}

const bool& Bubble::HitObject(const bool& hitFlag)
{
	if (updater == &Bubble::ShotUpdate && hitFlag)
	{
		Floating();
		return true;
	}
	return false;
}

const bool& Bubble::HitBubble(const bool& hitFlag, const bool& accelFlag)
{
	if (updater == &Bubble::PopUpdate)
	{
		vel = Vector2f(0, 0);
		return hitFlag;
	}
	if (hitFlag && accelFlag)
	{
		vel.y = -1.5f;
	}
	else
	{
		vel.y = -0.5f;
	}

	return hitFlag;
}

const bool& Bubble::CheckShotState()
{
	return (updater == &Bubble::ShotUpdate);
}

const bool& Bubble::CheckPopState()
{
	return (updater == &Bubble::PopUpdate);
}

void Bubble::Shot()
{
	ChangeAction("shot");
	updater = &Bubble::ShotUpdate;
}

void Bubble::Floating()
{
	vel.x = 0;
	vel.y = -0.5;
	ChangeAction("floating");
	updater = &Bubble::FloatingUpdate;
}

void Bubble::Pop()
{
	vel = Vector2f(0,0);
	ChangeAction("pop");
	turnFlag = false;				/// ´Ìª¸Ä‚Ì•`‰æ‚Ì”½“]‚ð–h‚¢‚Ä‚¢‚é
	updater = &Bubble::PopUpdate;
}

void Bubble::ShotUpdate()
{
	if (invCnt < 0)
	{
		Floating();
		vel.x = 0;
	}
	else
	{
		invCnt--;
		if (bubbleDir)
		{
			vel.x = -20.0;
		}
		else
		{
			vel.x = 20.0;
		}
	}
}

void Bubble::FloatingUpdate()
{
	
}

void Bubble::PopUpdate()
{
	deleteFlag = ProceedAnimFile();
}

void Bubble::Update()
{
	(this->*updater)();

	pos += vel;
}

void Bubble::Draw()
{
#ifdef _DEBUG
	DebugDraw();
#endif

	CharactorObject::Draw(bubbleImage);
}

void Bubble::ChangePop()
{
	if (updater != &Bubble::PopUpdate){ Pop(); }
	
}

void Bubble::DebugDraw()
{
	DrawString(100, 0, nowActionName.c_str(), 0xffffff);
	if (nowActionName == "floating")
	{
		DrawBox(GetRect().Left(), GetRect().Top(), GetRect().Right(), GetRect().Bottom(), 0xff0000, false);
	}
	else if (nowActionName == "shot")
	{
		DrawBox(ShotGetRect().Left(), ShotGetRect().Top(), ShotGetRect().Right(), ShotGetRect().Bottom(), 0xff0000, false);
	}
	else {}

}
