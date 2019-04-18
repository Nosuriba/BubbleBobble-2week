#include <DxLib.h>
#include "../Game.h"
#include "../CollisionDetector.h"
#include "Bubble.h"

Bubble::Bubble() : spitFrame(10), defSpeed(0.5f), colSpeed(1.5f)
{
	///	‰¼‚Ì‰Šú‰»
	Floating();
	nowCutIdx = 0;
	ReadActionFile("Action/bubble.act");		// –¼‘O‚Í‚±‚¤‚·‚é—\’è
	bubbleImage = DxLib::LoadGraph(actionData.imgFilePath.c_str());
	gameFlag = deleteFlag = false;

	vel.y = -(0.4f * (GetRand(6) + 1));
	invCnt  = spitFrame;
}

Bubble::Bubble(const bool& bubbleDir) : spitFrame(10), defSpeed(0.5f), colSpeed(1.5f)
{
	///	‰¼‚Ì‰Šú‰»
	Shot();
	nowCutIdx = 0;
	ReadActionFile("Action/bubble.act");		// –¼‘O‚Í‚±‚¤‚·‚é—\’è
	bubbleImage = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	this->bubbleDir = bubbleDir;
	gameFlag = true;
	deleteFlag = false;
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

void Bubble::MoveContact(const Rect & rcB)
{
	if (GetRect().Right() - (size.x / 3) > rcB.Left() &&
	    GetRect().Right() - (size.x / 3) < rcB.center.x && 
		CollisionDetector::SideCollCheck(GetRect(), rcB))
	{
		vel.x = -defSpeed / 2;
	}
	else if (GetRect().Left() + (size.x / 3) < rcB.Right() &&
			 GetRect().Left() + (size.x / 3) > rcB.center.x &&
			 CollisionDetector::SideCollCheck(GetRect(), rcB))
	{
		vel.x = defSpeed / 2;
	}
	else
	{
		vel.x = 0;
	}

	if (GetRect().Bottom() > rcB.Top() &&
		GetRect().Bottom() - (size.y / 3) < rcB.center.y &&
		CollisionDetector::CollCheck(GetRect(), rcB))
	{
		vel.y = -defSpeed * 2;
	}
	else if (GetRect().Top()  < rcB.Bottom() &&
			 GetRect().Top() + (size.y / 3) > rcB.center.y &&
			 CollisionDetector::CollCheck(GetRect(), rcB))
	{
		vel.y = defSpeed / 2;
	}
	else
	{
		vel.y = -defSpeed / 2;
	}
	
}

const Vector2f & Bubble::GetPos()
{
	return pos;
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

const bool& Bubble::HitBubble(const bool& hitFlag)
{
	if (updater == &Bubble::PopUpdate)
	{
		vel = Vector2f(0, 0);
		return hitFlag;
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

void Bubble::ChangePop()
{
	if (updater != &Bubble::PopUpdate) { Pop(); }

}

const bool& Bubble::CeilCheck()
{
	auto rtnFlag = false;
	if (pos.y < (size.y * 2) + (size.y / 2))
	{
		rtnFlag = true;
		if (pos.y < size.y + (size.y / 2))
		{
			vel.y = 0;
		}
	}
	return rtnFlag;
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
	turnFlag = false;				/// ´Ìª¸Ä‚Ì•`‰æ”½“]‚ð–h‚¢‚Ä‚¢‚é
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
	if (gameFlag)
	DebugDraw();
#endif

	CharactorObject::Draw(bubbleImage);
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
