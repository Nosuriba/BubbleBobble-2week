#include <DxLib.h>
#include "Wall.h"
#include "../ImageMng.h"

Wall::Wall()
{
}


Wall::~Wall()
{
}

void Wall::Update()
{
}

void Wall::Draw()
{
	DxLib::DrawGraph(pos.x, pos.y, ImageMng::GetInstance().ImgGetID(fileName, divCnt, size)[chipCnt], true);
}

Rect Wall::GetRect()
{
	auto center = Vector2(pos.x + (size.x / 2), pos.y + (size.y / 2));
	auto rectSize = Size(size.x, size.y);

	return Rect(center, rectSize);
}
