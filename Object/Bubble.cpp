#include <DxLib.h>
#include "Bubble.h"

Bubble::Bubble()
{
	///	���̏�����
	Spit();
	nowCutIdx = 0;
	ReadActionFile("Action/bubble.act");		// ���O�͂�������\��
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

