#include "../Input.h"
#include "Block.h"

Block::Block()
{
	pos	   = vel =  Vector2f(0, 0);
	divCnt = size = Vector2(0, 0);
	fileName = "";
}

Block::Block(int groundLine)
{
	pos = vel = Vector2f(0, 0);
	divCnt = size = Vector2(0, 0);
	fileName = "";
	this->groundLine = groundLine;
}

Block::~Block()
{
}

void Block::Update(const Input & p)
{
	Draw();
}

void Block::Draw()
{
	DrawBox(GetRect().Left(), GetRect().Top(), GetRect().Right(), GetRect().Bottom(), 0xff0000, true);
}

Rect Block::GetRect()
{
	auto center = Vector2(pos.x + (size.x / 2), pos.y + (size.y / 2));
	auto rectSize = Size(size.x, size.y);

	return Rect(center, rectSize);
}

void Block::InitAnim(void)
{

}

void Block::AddAnim(std::string animName, const Vector2 & id, int frame, int interval)
{
	
}
