#include <DxLib.h>
#include "Bubble.h"

Bubble::Bubble()
{
	///	‰¼‚Ì‰Šú‰»
	Spit();
	nowCutIdx = 0;
	ReadActionFile("Action/bubble.act");		// –¼‘O‚Í‚±‚¤‚·‚é—\’è
	bubbleImage = DxLib::LoadGraph(actionData.imgFilePath.c_str());
	pos = vel = Vector2f(0, 0);
	size = Vector2();
}

Bubble::~Bubble()
{
}

void Bubble::Spit()
{
}

void Bubble::Floating()
{
}

void Bubble::SpitUpdate()
{
}

void Bubble::FloatingUpdate()
{
}

void Bubble::Update()
{
	(this->*updater)();
}

void Bubble::Draw()
{
}

Rect Bubble::GetRect()
{
	auto center = Vector2(pos.x + (size.x / 2), pos.y + (size.x / 2));
	auto rectSize = Size(size.x, size.y);

	return Rect(center, rectSize);
}

