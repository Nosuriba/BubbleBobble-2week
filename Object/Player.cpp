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
}

Player::Player(int groundLine)
{
	pos		 = vel  = Vector2f(0, 0);
	divCnt	 = size = Vector2(0, 0);
	fileName = "";
	this->groundLine = groundLine;

	jumpFlag = groundFlag = dieFlag = false;
}

Player::~Player()
{
}

void Player::Update(const Input & p)
{
	Move(p);
	if (pos.y + size.y < groundLine)
	{
		vel.y += 0.5f;
	}
	else
	{
		vel.y = 0;
		pos.y = groundLine - size.y;
	}

	pos += vel;
}

void Player::Draw()
{
	DxLib::DrawGraph(pos.x, pos.y, LpImageMng.ImgGetID(fileName, divCnt, size)[animCnt], true);
}

void Player::Move(const Input & p)
{
	if (p.IsTrigger(PAD_INPUT_10))
	{
  		vel.y -= 15.0f;
	}
}
