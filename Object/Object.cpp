#include "../ImageMng.h"
#include "../Input.h"
#include "Object.h"

Object::Object()
{

}

Object::Object(int groundLine)
{
}


Object::~Object()
{
}

void Object::Update(const Input & p)
{
	/// ‰½‚à‘‚©‚È‚¢
}

void Object::Draw()
{
	/// ‰½‚à‘‚©‚È‚¢
}

void Object::Init(std::string fileName, const Vector2f & pos,
				const Vector2 & divCnt, const Vector2 & divOffset,
			    const Vector2 & size)
{
	
	this->fileName  = fileName;
	this->pos	    = pos;
	this->divCnt	= divCnt;
	this->divOffset = divOffset;
	this->size	    = size;

	/// •`‰æ‚·‚é‰æ‘œ‚ÌˆÊ’uİ’è
	animCnt = (divCnt.x * divOffset.y) + divOffset.x;
}
