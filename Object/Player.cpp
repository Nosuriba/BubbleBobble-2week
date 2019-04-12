#include <DxLib.h>
#include <cmath>
#include "CharactorObject.h"
#include "Player.h"
#include "../ImageMng.h"
#include "../Input.h"
#include "../Game.h"

Player::Player()
{
}

Player::Player(int groundLine)
{
	nowActionName = "idle";
	nowCutIdx	  = 0;
	ReadActionFile("Action/player.act");
	playerImg	 = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	pos	 = vel = Vector2f(0, 0);
	size = Vector2(0, 0);
	turnFlag = false;
	jumpFlag = dieFlag = hitFlag = false;

	this->groundLine = groundLine;
	updater = &Player::Idle;
}

Player::~Player()
{
}

Rect Player::GetRect()
{
	auto center = Vector2(pos.x + (size.x / 2), pos.y + (size.y / 2));
	auto rectSize = Size(size.x, size.y);

	return Rect(center, rectSize);
}



bool Player::HitWall(bool hitFlag, const Rect& rcB)
{
	this->hitFlag = hitFlag;
	if (hitFlag)
	{
		/// 壁の当たった場所によって、位置補正を行っている
		pos.x = (turnFlag ? pos.x = rcB.Right() : pos.x = rcB.Left() - size.x);
	}

	return this->hitFlag;
}

bool Player::HitGround(bool groundFlag, const Rect& rcB)
{
	/// 落下中にブロックの上に乗った時の処理
	if (groundFlag && vel.y >= 0.0f)
	{
 		this->vel.y		 = 0;
		this->groundLine = rcB.Top() + 1;		/// 床に少しめり込むようにしている。
	}
	else
	{
		this->groundLine = Game::GetInstance().GetScreenSize().y + size.y * 2;			// いずれ、画面の一番下を指定することになる(画面外指定)
		
	}

	return groundFlag;
}

void Player::Idle(const Input & p)
{
	if (p.IsPressing(PAD_INPUT_RIGHT) ||
		p.IsPressing(PAD_INPUT_LEFT))
	{
		updater = &Player::Run;
		ChangeAction("run");
		return;
	}
	
	if (p.IsPressing(PAD_INPUT_6))
	{
		ChangeAction("eat");
		updater = &Player::Shot;
	}

	/// 地面についているかの判定
	if (OnGround())
	{
		vel = Vector2f(0,0);
		pos.y = groundLine - size.y;
		if (p.IsTrigger(PAD_INPUT_5))
		{
			updater = &Player::Jump;
			ChangeAction("jump");
			jumpFlag   = true;
			vel.y	   -= 14.0f;
			return;
		}
	}
	else
	{
		updater = &Player::Jump;
		ChangeAction("jump");
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
			vel.x = 7.0f;
		}
		else if (p.IsPressing(PAD_INPUT_LEFT))
		{
			turnFlag = true;
			vel.x = -7.0f;
		}
		else
		{
			ChangeAction("idle");
			vel.x = 0;
			updater = &Player::Idle;
		}
	}
	else
	{
		vel.x = 0;
	}
	
	/// 地面についているかの判定
	if (OnGround())
	{
		vel.y = 0;
		// pos.y = groundLine - size.y;
		if (p.IsTrigger(PAD_INPUT_5))
		{
			ChangeAction("jump");
			jumpFlag   = true;
			vel.y	  -= 14.0f;
			updater    = &Player::Jump;
		}
	}
	else
	{
		updater = &Player::Jump;
		ChangeAction("jump");
	}
	
	ProceedAnimFile();
}

void Player::Jump(const Input & p)
{
	/// 地面についているかの判定
	if (OnGround())
	{
		updater = &Player::Idle;
		ChangeAction("idle");
		vel.y = 0;
		pos.y = groundLine - size.y;
		return;
	}
	else
	{
		///	落下処理
		vel.y  = (vel.y < 0.5f ? vel.y += 0.7f : vel.y = 5.0);
	}

	if (!hitFlag)
	{
		if (p.IsPressing(PAD_INPUT_RIGHT))
		{
			turnFlag = false;
			vel.x = 7.0f;
		}
		else if (p.IsPressing(PAD_INPUT_LEFT))
		{
			turnFlag = true;
			vel.x = -7.0f;
		}
		else
		{
			vel.x = 0;
		}
	}
	else
	{
		vel.x = 0;
	}
	ProceedAnimFile();
}

void Player::Shot(const Input & p)
{
	if (p.IsPressing(PAD_INPUT_6))
	{

	}
	else
	{
		ChangeAction("idle");
		updater = &Player::Idle;
	}

	/// 地面についているかの判定
	if (OnGround())
	{
		vel.y = 0;
		if (p.IsTrigger(PAD_INPUT_5))
		{
			ChangeAction("jump");
			jumpFlag = true;
			vel.y -= 12.0f;
			updater = &Player::Jump;
		}
	}
	else
	{
		updater = &Player::Jump;
		ChangeAction("jump");
	}
	ProceedAnimFile();
}

bool Player::OnGround()
{
	if (pos.y + size.y >= groundLine)
	{
		jumpFlag = false;
		return true;
	}
	return false;
}


void Player::Update(const Input & p)
{
	(this->*updater)(p);

	if (pos.y > Game::GetInstance().GetScreenSize().y)
	{
		pos.y = -(size.y);
	}
	
	pos += vel;
}

void Player::Draw()
{
	DebugDraw();
	CharactorObject::Draw(playerImg);
}

void Player::DebugDraw()
{
	DrawString(0, 0, nowActionName.c_str(), 0xffffff);

	DrawBox(GetRect().Left(), GetRect().Top(), GetRect().Right(), GetRect().Bottom(), 0xff0000, true);
}