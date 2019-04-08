#include <DxLib.h>
#include <cmath>
#include "../ImageMng.h"
#include "../Input.h"
#include "Player.h"

Player::Player()
{
	pos	     = vel  = Vector2f(0, 0);
	divCnt   = size = Vector2(0, 0);
	fileName = "";

	airFlag = groundFlag = dieFlag = false;
	invCnt	 = 0;
}

Player::Player(int groundLine)
{
	pos		 = vel  = Vector2f(0, 0);
	divCnt	 = size = Vector2(0, 0);
	fileName = "";
	this->groundLine = groundLine;

	airFlag = groundFlag = dieFlag = false;
	invCnt	 = 0;
}

Player::~Player()
{
}

void Player::InitAnim(void)
{
	AddAnim("idle", Vector2(0, 0), 3, 30);
	AddAnim("run",  Vector2(3,0), 5, 10);
	AddAnim("jump", Vector2(7,0), 4, 10);
	AddAnim("randing", Vector2(11,0), 4, 10);
	//AddAnim();
}

void Player::AddAnim(std::string animName, const Vector2 & id, int frame, int interval)
{
	animType[animName][static_cast<int>(ANIM::START)]    = ((id.y * divCnt.x) + id.x);
	animType[animName][static_cast<int>(ANIM::FRAME)]	 = frame;
	animType[animName][static_cast<int>(ANIM::INTERVAL)] = interval;
}

void Player::Anim()
{
	invCnt++;
	chipCnt = animType[animName][static_cast<int>(ANIM::START)] + 
			 (invCnt / animType[animName][static_cast<int>(ANIM::INTERVAL)]) %
					   animType[animName][static_cast<int>(ANIM::FRAME)];

	if (!groundFlag)
	{
		/// アニメーションを止めている。
		SetAnim("jump");
		chipCnt = animType[animName][static_cast<int>(ANIM::START)];
	}
	else
	{
		SetAnim("run");
	}

}

void Player::Move(const Input & p)
{
	if (p.IsPressing(PAD_INPUT_RIGHT))
	{
		vel.x = 1.0f;
	}
	else if (p.IsPressing(PAD_INPUT_LEFT))
	{
		vel.x = -1.0f;
	}
	else
	{
		vel.x = 0;
	}

}

void Player::Jump(const Input & p)
{
	if (!airFlag)
	{
		if (p.IsTrigger(PAD_INPUT_10))
		{
 			airFlag = true;
			groundFlag = false;
			vel.y = 0;
			vel.y -= 12.0f;
		}
	}
	
}

void Player::Fall()
{
	if (groundFlag)
	{
		vel.y = 0;
		pos.y = groundLine - size.y;
	}
	else
	{
		vel.y += 0.5f;
	}
}


void Player::Update(const Input & p)
{
	// (this->*updater)(p);
	Anim();
	Jump(p);
	Move(p);
	Fall();
	if (pos.y + size.y > groundLine)
	{
		airFlag = false;
 		groundFlag = true;
	}
	pos += vel;
}

void Player::Draw()
{
	DxLib::DrawTurnGraph(pos.x, pos.y, LpImageMng.ImgGetID(fileName, divCnt, size)[chipCnt], true);
}

