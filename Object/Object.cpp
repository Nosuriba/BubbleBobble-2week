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
	/// ���������Ȃ�
}

void Object::Draw()
{
	/// ���������Ȃ�
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

	/// �`�悷��摜�̈ʒu�ݒ�
	chipCnt = (divCnt.x * divOffset.y) + divOffset.x;

	InitAnim();
}

void Object::SetAnimName(std::string animName)
{
	if (animType.find(animName) == animType.end())
	{
		return;
	}
	if (this->animName != animName)
	{
		invCnt = 0;
		this->animName = animName;
	}
}
