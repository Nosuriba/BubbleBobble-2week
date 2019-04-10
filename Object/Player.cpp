#include <DxLib.h>
#include <cmath>
#include "../ImageMng.h"
#include "../Input.h"
#include "CharactorObject.h"
#include "Player.h"

Player::Player()
{
}

Player::Player(int groundLine)
{
	actionName	  = "idle";
	nowActionName = actionName.c_str();
	nowCutIdx	  = 0;
	ReadActionFile("Action/player.act");
	playerImg	 = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	pos		 = vel  = Vector2f(0, 0);
	divCnt	 = size = Vector2(0, 0);
	turnFlag = false;
	jumpFlag = groundFlag = dieFlag = runFlag = hitFlag = false;

	this->groundLine = groundLine;
	updater = &Player::Idle;
}

Player::~Player()
{
}

void Player::CheckHit(bool hitFlag)
{
	this->hitFlag = hitFlag;
}

void Player::Idle(const Input & p)
{
	if (p.IsPressing(PAD_INPUT_RIGHT) ||
		p.IsPressing(PAD_INPUT_LEFT))
	{
		updater = &Player::Run;
		actionName = "run";
		ChangeAction(actionName.c_str());
		turnFlag = (p.IsTrigger(PAD_INPUT_RIGHT) ? turnFlag = false 
												 : turnFlag = true);
		runFlag = true;
		return;
	}

	if (groundFlag)
	{
		vel = Vector2f(0,0);
		pos.y = groundLine - size.y;
		if (p.IsTrigger(PAD_INPUT_10))
		{
			updater = &Player::Jump;
			actionName = "jump";
			ChangeAction(actionName.c_str());
			jumpFlag   = true;
			groundFlag = false;
			vel.y	   -= 12.0f;
			return;
		}
	}
	else
	{
		vel.y += 0.7f;
	}

	ProceedAnimFile();
}

void Player::Run(const Input & p)
{
	
	if (!hitFlag)
	{
		if (p.IsPressing(PAD_INPUT_RIGHT))
		{
			turnFlag = false;
			vel.x = 5.0f;
		}
		else if (p.IsPressing(PAD_INPUT_LEFT))
		{
			turnFlag = true;
			vel.x = -5.0f;
		}
		else
		{
			actionName = "idle";
			ChangeAction(actionName.c_str());
			runFlag = false;
			vel.x = 0;
			updater = &Player::Idle;
		}
	}
	if (groundFlag)
	{
		vel.y = 0;
		pos.y = groundLine - size.y;
		if (p.IsTrigger(PAD_INPUT_10))
		{
			actionName = "jump";
			ChangeAction(actionName.c_str());
			jumpFlag   = true;
			groundFlag = false;
			vel.y	  -= 12.0f;
			updater    = &Player::Jump;
		}
	}
	else
	{
		vel.y += 0.7f;
	}
	
	ProceedAnimFile();
}

void Player::Jump(const Input & p)
{
	if (groundFlag)
	{
		actionName = "idle";
		ChangeAction(actionName.c_str());
		updater = &Player::Idle;
		vel.y = 0;
		pos.y = groundLine - size.y;
		return;
	}
	else
	{
		vel.y += 0.7f;
	}

	if (p.IsPressing(PAD_INPUT_RIGHT))
	{
		turnFlag = false;
		vel.x = 5.0f;
	}
	else if (p.IsPressing(PAD_INPUT_LEFT))
	{
		turnFlag = true;
		vel.x = -5.0f;
	}
	else
	{
		vel.x = 0;
	}

	ProceedAnimFile();
}

void Player::DebugDraw()
{
	DrawString(0, 0, actionName.c_str(), 0xffffff);
	
	DrawBox(GetRect().Left(), GetRect().Top(), GetRect().Right(), GetRect().Bottom(), 0xff0000, true);
}


void Player::Update(const Input & p)
{
	(this->*updater)(p);
	if (pos.y + size.y > groundLine)
	{
		jumpFlag = false;
 		groundFlag = true;
	}
	pos += vel;
}

void Player::Draw()
{
	DebugDraw();
	CharactorObject::Draw(playerImg);
}

Rect Player::GetRect()
{
	auto center = Vector2(pos.x + (size.x / 2), pos.y + (size.y / 2));
	auto rectSize = Size(size.x, size.y);

	return Rect(center, rectSize);
}

