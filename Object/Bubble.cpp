#include <DxLib.h>
#include "Bubble.h"

Bubble::Bubble()
{
	///	仮の初期化
	Spit();
	nowCutIdx = 0;
	ReadActionFile("Action/bubble.act");		// 名前はこうする予定
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

