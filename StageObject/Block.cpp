#include "Block.h"
#include "../Input.h"
#include "../ImageMng.h"

Block::Block()
{
	pos	   =  Vector2f(0, 0);
	divCnt = size = Vector2(0, 0);
	fileName = "";
}

Block::~Block()
{
}

void Block::Update(const Input & p)
{
	
}

void Block::Draw()
{
	DxLib::DrawGraph(pos.x, pos.y, ImageMng::GetInstance().ImgGetID(fileName, divCnt, size)[chipCnt], true);
}

Rect Block::GetRect()
{
	auto center = Vector2(pos.x + (size.x / 2), pos.y + (size.y / 2));
	auto rectSize = Size(size.x, size.y);

	return Rect(center, rectSize);
}
