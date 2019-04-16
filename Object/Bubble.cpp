#include <DxLib.h>
#include "Bubble.h"

Bubble::Bubble(const bool& bubbleDir) : spitFrame(10)
{
	///	âºÇÃèâä˙âª
	Shot();
	nowCutIdx = 0;
	ReadActionFile("Action/bubble.act");		// ñºëOÇÕÇ±Ç§Ç∑ÇÈó\íË
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

shared_Bubble Bubble::DeleteBubble(const shared_Bubble & itr)
{
	if (deleteFlag)
	{
		return nullptr;		// ñAÇÃçÌèú
	}
	return itr;				// ÇªÇÃÇ‹Ç‹ï‘Ç∑
}

bool Bubble::HitPlayer(const bool hitFlag)
{
	if (updater == &Bubble::Floating && hitFlag)
	{
		Pop();
		return true;
	}
	return false;
}

bool Bubble::HitEnemy(const bool hitFlag)
{
	if (updater == &Bubble::ShotUpdate && hitFlag)
	{
		deleteFlag = false;
		return true;
	}
	return false;
}

bool Bubble::HitObject(const bool hitFlag)
{
	if (updater == &Bubble::ShotUpdate && hitFlag)
	{
		Floating();
		return true;
	}
	return false;
}

bool Bubble::HitBubble(const bool hitFlag)
{
	if (hitFlag)
	{
		vel.y = -1.0f;
		return true;
	}
	return false;
}

bool Bubble::CheckShotState()
{
	if (updater == &Bubble::ShotUpdate) { return true;}
	return false;
}

void Bubble::Shot()
{
	ChangeAction("shot");
	updater = &Bubble::ShotUpdate;
}

void Bubble::Floating()
{
	vel.x = 0;
	ChangeAction("floating");
	updater = &Bubble::FloatingUpdate;
}

void Bubble::Pop()
{
	vel = Vector2f(0,0);
	ChangeAction("pop");
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
	vel.y = -0.5;
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
	CharactorObject::Draw(bubbleImage);
}
