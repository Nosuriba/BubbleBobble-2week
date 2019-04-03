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
