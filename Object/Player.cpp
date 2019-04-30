#include <DxLib.h>
#include <cmath>
#include "Player.h"
#include "../Input.h"
#include "../Game.h"
#include "../AudioMng.h"
#include "../CollisionDetector.h"

Player::Player() : shotFrame(5)
{
}

Player::Player(int groundLine) : shotFrame(5)
{
	Idle();
	nowCutIdx	  = 0;
	ReadActionFile("Action/player.act");
	playerImg	  = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	this->groundLine = startPos = groundLine + 1;		/// 床にめり込むように補正している。
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

bool Player::GetTurnFlag()
{
	return turnFlag;
}

const Vector2f& Player::GetPos()
{
	return pos;
}

bool Player::HitEnemy(const Rect & eRect, bool eAlive)
{
	auto hitCheck = CollisionDetector::CollCheck(GetRect(), eRect);

	if (updater != &Player::DieUpdate && hitCheck && eAlive)
	{
		Die();
		return true;
	}

	return false;
}

bool Player::HitWall(const Rect& wRect)
{
	this->hitFlag = CollisionDetector::SideCollCheck(GetRect(), wRect);
	if (hitFlag)
	{
		/// 壁の当たった場所によって、位置補正を行っている
		vel.x = 0;
		pos.x = (turnFlag ? wRect.Right() : wRect.Left() - size.x);
	}

	return this->hitFlag;
}

bool Player::HitGround(const Rect& bRect)
{
	auto underCheck = CollisionDetector::UnderCollCheck(GetRect(), bRect);
	/// 落下中にブロックの上に乗った時の処理
	if (underCheck && vel.y >= 0.f)
	{
 		this->vel.y		 = 0;
		this->groundLine = bRect.Top() + 1;		/// 床に少しめり込むようにしている。
	}
	else
	{
		//// 天井に乗ってしまうので、すぐに直すようにする
		this->groundLine = Game::GetInstance().GetScreenSize().y + (size.y * 2);
	}
	return underCheck;
}

void Player::StepBubble()
{
	AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().bubble);
	AudioMng::GetInstance().ChangeVolume(80, AudioMng::GetInstance().GetSound().bubble);
	vel.y = -12.0f;
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
	nowCutIdx = 2;
	updater   = &Player::JumpUpdate;
	jumpFlag  = true;
}

void Player::Shot()
{
	ChangeAction("eat");
	updater = &Player::ShotUpdate;
}

void Player::Die()
{
	ChangeAction("die");
	vel.x = 0;
	updater = &Player::DieUpdate;
}

void Player::IdleUpdate(const Input & p)
{
	if (p.IsPressing(PAD_INPUT_RIGHT) ||
		p.IsPressing(PAD_INPUT_LEFT))
	{
		Run();
		return;
	}

	if (p.IsTrigger(PAD_INPUT_2))
	{
		shotFlag = true;
		AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().shot);
		AudioMng::GetInstance().ChangeVolume(50, AudioMng::GetInstance().GetSound().shot);
		Shot();
	}

	/// 地面についているかの判定
	if (OnGround())
	{
		vel = Vector2f(0, 0);
		pos.y = groundLine - size.y;
		if (p.IsTrigger(PAD_INPUT_1))
		{
			vel.y -= 14.0f;
			AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().jump);
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
			vel.x = charSpeed;
		}
		else if (p.IsPressing(PAD_INPUT_LEFT))
		{
			turnFlag = true;
			vel.x = -charSpeed;
		}
		else
		{
			vel.x = 0;
			Idle();
		}
	}

	if (p.IsTrigger(PAD_INPUT_2))
	{
		shotFlag = true;
		AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().shot);
		AudioMng::GetInstance().ChangeVolume(50, AudioMng::GetInstance().GetSound().shot);
		Shot();
	}

	/// 地面についているかの判定
	if (OnGround())
	{
		vel.y = 0;
		if (p.IsTrigger(PAD_INPUT_1))
		{
			AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().jump);
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

	if (p.IsTrigger(PAD_INPUT_2))
	{
		shotFlag = true;
		AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().shot);
		AudioMng::GetInstance().ChangeVolume(50,AudioMng::GetInstance().GetSound().shot);
		Shot();
	}

	if (!hitFlag)
	{
		if (p.IsPressing(PAD_INPUT_RIGHT))
		{
			turnFlag = false;
			vel.x = charSpeed;
		}
		else if (p.IsPressing(PAD_INPUT_LEFT))
		{
			turnFlag = true;
			vel.x = -charSpeed;
		}
		else
		{
			vel.x = 0;
		}
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
			vel.x = charSpeed;
		}
		else if (p.IsPressing(PAD_INPUT_LEFT))
		{
			turnFlag = true;
			vel.x = -charSpeed;
		}
		else
		{
			vel.x = 0;
		}
	}

	if (ProceedAnimFile())
	{
		Idle();
	}
}

void Player::DieUpdate(const Input & p)
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

	/// 死亡時のｱﾆﾒｰｼｮﾝが終わった時、待機状態にする
	if (ProceedAnimFile())
	{
		/// ここにﾌﾟﾚｲﾔｰの残機などの処理を追加するといい
		Idle();
		vel = Vector2f(0, 0);
		pos = Vector2f(size.x * 2, startPos - size.y);
	}
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
	// DrawString(0, 0, nowActionName.c_str(), 0xffffff);
	DrawBox(GetRect().Left(), GetRect().Top(), GetRect().Right(), GetRect().Bottom(), 0xff0000, false);
}