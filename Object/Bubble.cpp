#include <DxLib.h>
#include "Bubble.h"

Bubble::Bubble(const bool& bubbleDir) : spitFrame(10)
{
	///	‰¼‚Ì‰Šú‰»
	Spit();
	nowCutIdx = 0;
	ReadActionFile("Action/bubble.act");		// –¼‘O‚Í‚±‚¤‚·‚é—\’è
	bubbleImage = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	this->bubbleDir = bubbleDir;
	pos		= vel = Vector2f(0, 0);
	size	= Vector2();
	invCnt  = spitFrame;
}

Bubble::~Bubble()
{
	DxLib::DeleteGraph(bubbleImage);
}

void Bubble::Spit()
{
	ChangeAction("shot");
	updater = &Bubble::SpitUpdate;
}

void Bubble::Floating()
{
	ChangeAction("floating");
	updater = &Bubble::FloatingUpdate;
}

void Bubble::SpitUpdate()
{
	if (invCnt < 0)
	{
		Floating();
		vel.x = 0;
		return;
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

void Bubble::Update()
{
	(this->*updater)();

	pos += vel;
}

void Bubble::Draw()
{
	CharactorObject::Draw(bubbleImage);
}

Rect Bubble::GetRect()
{
	auto center = Vector2(pos.x + (size.x / 2), pos.y + (size.x / 2));
	auto rectSize = Size(size.x, size.y);

	return Rect(center, rectSize);
}

