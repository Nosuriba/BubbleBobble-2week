#include "../ImageMng.h"
#include "../Input.h"
#include "Object.h"

Object::Object()
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

void Object::Init(std::string fileName, const Vector2 & pos, const Vector2 & divCnt, const Vector2 & size)
{
	ImageMng::GetInstance().ImgGetID(fileName, divCnt, size);
	this->fileName = fileName;
	this->pos	   = pos;
	this->size	   = size;

}
