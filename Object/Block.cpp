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
}

void Block::Draw()
{
}

void Block::InitAnim(void)
{

}

void Block::AddAnim(std::string animName, const Vector2 & id, int frame, int interval)
{
	animType[animName][static_cast<int>(ANIM::START)]   = ((id.y * divCnt.x) + id.x);
	animType[animName][static_cast<int>(ANIM::FRAME)]   = frame;
	animType[animName][static_cast<int>(ANIM::INTERVAL)] = interval;
}
