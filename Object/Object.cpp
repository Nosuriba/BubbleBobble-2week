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
	/// 何も書かない
}

void Object::Draw()
{
	/// 何も書かない
}

void Object::Init(std::string fileName, std::string animName,
				  const Vector2f & pos, const Vector2 & divCnt, const Vector2 & size)
{
	/// アニメーションの設定
	InitAnim();
	SetAnim(animName);

	this->fileName  = fileName;
	this->pos	    = pos;
	this->divCnt	= divCnt;
	this->size	    = size;

	/// 描画する画像の位置設定
	chipCnt = animType[animName][static_cast<int>(ANIM::START)];

	
}

void Object::SetAnim(std::string animName)
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
