#include <DxLib.h>
#include "../ImageMng.h"
#include "../Input.h"
#include "Player.h"

Player::Player()
{
	pos	     = vel  = Vector2f(0, 0);
	divCnt   = size = Vector2(0, 0);
	fileName = "";

	jumpFlag = groundFlag = dieFlag = false;
	jumpCnt  = 0;
	animCnt  = 0;
	invCnt	 = 0;
}

Player::Player(int groundLine)
{
	pos		 = vel  = Vector2f(0, 0);
	divCnt	 = size = Vector2(0, 0);
	fileName = "";
	this->groundLine = groundLine;

	jumpFlag = groundFlag = dieFlag = false;
	jumpCnt  = 0;
	animCnt  = 0;
	invCnt	 = 0;
}

Player::~Player()
{
}

void Player::Anim()
{
	invCnt++;
	animCnt = ((invCnt / 30) % 3);			/// アニメーションがバグっているので修正する
	chipCnt = chipCnt + animCnt;
}

void Player::Jump(const Input & p)
{
	if (p.IsTrigger(PAD_INPUT_10))
	{
		if (!jumpFlag)
		{
			jumpCnt++;
			groundFlag = false;
			vel.y = 0;
			vel.y -= 12.0f;
		}
	}

	if (jumpCnt >= 2)
	{
		jumpFlag = true;
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
	Anim();
	Jump(p);
	Fall();
	if (pos.y + size.y > groundLine)
	{
		jumpCnt	   = 0;
		jumpFlag   = false;
 		groundFlag = true;
	}
	pos += vel;
}

void Player::Draw()
{
	DxLib::DrawTurnGraph(pos.x, pos.y, LpImageMng.ImgGetID(fileName, divCnt, size)[chipCnt], true);
}

