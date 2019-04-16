#include <DxLib.h>
#include <cmath>
#include "CharactorObject.h"
#include "Player.h"
#include "../Input.h"
#include "../Game.h"

Player::Player() : shotFrame(5)
{
}

Player::Player(int groundLine) : shotFrame(5)
{
	Idle();
	nowCutIdx	  = 0;
	ReadActionFile("Action/player.act");
	playerImg	  = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	this->groundLine = groundLine + 1;		/// 床にめり込むように補正している。
	pos	 = vel = Vector2f(0, 0);
	size = Vector2(0, 0);
	jumpFlag = dieFlag = hitFlag = turnFlag = shotFlag = false;
	
}

Player::~Player()
{
	DxLib::DeleteGraph(playerImg);
}

Rect Player::GetRect()
{
	auto center   = Vector2(pos.x + (size.x / 2), pos.y + (size.y / 2));
	auto rectSize = Size(size.x, size.y);

	return Rect(center, rectSize);
}

const bool & Player::GetTurnFlag()
{
	return turnFlag;
}

const Vector2f& Player::GetPos()
{
	return pos;
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
		this->groundLine = Game::GetInstance().GetScreenSize().y + (size.y * 2);
	}
	return groundFlag;
}

void Player::StepBubble(bool stepFlag, const Input & p)
{
	if (stepFlag && p.IsPressing(PAD_INPUT_5))
	{
		vel.y = -10.0f;
	}
}

bool Player::ShotCheck()
{
	if (shotFlag)
	{
		invCnt = shotFrame;
		shotFlag = false;
		return true;
	}
	return false;
}

void Player::Idle()
{
	ChangeAction("idle");
	updater = &Player::IdleUpdate;
}

void Player::Run()
{
	ChangeAction("run");
	updater = &Player::RunUpdate;
}

void Player::Jump()
{
	ChangeAction("jump");
	updater = &Player::JumpUpdate;
	jumpFlag = true;
}

void Player::Shot()
{
	ChangeAction("eat");
	updater = &Player::ShotUpdate;
}

void Player::Die()
{
}

void Player::IdleUpdate(const Input & p)
{
	if (p.IsPressing(PAD_INPUT_RIGHT) ||
		p.IsPressing(PAD_INPUT_LEFT))
	{
		Run();
		return;
	}

	if (p.IsTrigger(PAD_INPUT_6))
	{
		shotFlag = true;
		Shot();
	}

	/// 地面についているかの判定
	if (OnGround())
	{
		vel = Vector2f(0, 0);
		pos.y = groundLine - size.y;
		if (p.IsTrigger(PAD_INPUT_5))
		{
			vel.y -= 14.0f;
			Jump();
			return;
		}
	}
	else
	{
		Jump();
	}

	ProceedAnimFile();
}

void Player::RunUpdate(const Input & p)
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
			vel.x = 0;
			Idle();
		}
	}
	else
	{
		vel.x = 0;
	}

	if (p.IsTrigger(PAD_INPUT_6))
	{
		shotFlag = true;
		Shot();
	}

	/// 地面についているかの判定
	if (OnGround())
	{
		vel.y = 0;
		if (p.IsTrigger(PAD_INPUT_5))
		{
			vel.y -= 14.0f;
			Jump();
		}
	}
	else
	{
		Jump();
	}

	ProceedAnimFile();
}

void Player::JumpUpdate(const Input & p)
{
	/// 地面についているかの判定
	if (OnGround())
	{
		vel.y = 0;
		pos.y = groundLine - size.y;
		Idle();
		return;
	}
	else
	{
		///	落下処理
		vel.y = (vel.y < 0.5f ? vel.y += 0.7f : vel.y = 5.0);
	}

	if (p.IsTrigger(PAD_INPUT_6))
	{
		shotFlag = true;
		Shot();
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

void Player::ShotUpdate(const Input & p)
{
	/// 地面についているかの判定
	if (OnGround())
	{
		vel.y = 0;
		pos.y = groundLine - size.y;
	}
	else
	{
		///	落下処理
		vel.y = (vel.y < 0.5f ? vel.y += 0.7f : vel.y = 5.0);
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

	if (ProceedAnimFile())
	{
		Idle();
	}
}

void Player::DieUpdate(const Input & p)
{
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

	invCnt = (invCnt < 0 ? invCnt = 0  :  invCnt -= 1);

	if (pos.y > Game::GetInstance().GetScreenSize().y)
	{
		pos.y = -(size.y);
	}
	
	pos += vel;
}

void Player::Draw()
{
	CharactorObject::Draw(playerImg);

#ifdef _DEBUG
	DebugDraw();
#endif 

}

void Player::DebugDraw()
{
	DrawString(0, 0, nowActionName.c_str(), 0xffffff);
	DrawBox(GetRect().Left(), GetRect().Top(), GetRect().Right(), GetRect().Bottom(), 0xff0000, false);
}