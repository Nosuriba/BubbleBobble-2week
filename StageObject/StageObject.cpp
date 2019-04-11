#include "StageObject.h"


StageObject::StageObject()
{
}


StageObject::~StageObject()
{
}

void StageObject::Update()
{
}

void StageObject::Draw()
{
}

Rect StageObject::GetRect()
{
	return Rect();
}

void StageObject::Init(std::string fileName, const Vector2f & pos, const Vector2 & divCnt, const Vector2 & divOffset, const Vector2 & size)
{
	this->fileName = fileName;
	this->pos = pos;
	this->divCnt = divCnt;
	this->size = size;

	chipCnt = (divOffset.y * divCnt.x) + divOffset.x;
}

Vector2f StageObject::GetPos() const
{
	return pos;
}
