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

	jumpFlag = groundFlag = dieFlag = turnFlag = runFlag = false;
	invCnt	 = 0;
}

Player::Player(int groundLine)
{
	pos		 = vel  = Vector2f(0, 0);
	divCnt	 = size = Vector2(0, 0);
	fileName = "";
	turnFlag = true;
	jumpFlag = groundFlag = dieFlag = runFlag = false;
	invCnt	 = 0;

	this->groundLine = groundLine;
	SetAnim("idle");
	updater = &Player::Idle;
}

Player::~Player()
{
}

void Player::InitAnim(void)
{
	AddAnim("idle", Vector2(0, 0), 6, 30);
	AddAnim("run",  Vector2(3,0), 5, 30);
	AddAnim("jump", Vector2(8,0), 4, 20);
	AddAnim("randing", Vector2(12,0), 4, 10);
	//AddAnim();
}

void Player::AddAnim(std::string animName, const Vector2 & id, int frame, int interval)
{
	animType[animName][static_cast<int>(ANIM::START)]    = ((id.y * divCnt.x) + id.x);
	animType[animName][static_cast<int>(ANIM::FRAME)]	 = frame;
	animType[animName][static_cast<int>(ANIM::INTERVAL)] = interval;
}

void Player::ChangeAnim()
{
	/// アニメーションの設定(モーションが増える可能性があるので、今後修正予定)
	if (jumpFlag)
	{
		SetAnim("jump");
	}
	else
	{
		if (runFlag)
		{
			SetAnim("run");
		}
		else
		{
			SetAnim("idle");
		}
	}
}

void Player::Anim()
{
	ChangeAnim();
	invCnt++;
	chipCnt = animType[animName][static_cast<int>(ANIM::START)] + 
			 (invCnt / animType[animName][static_cast<int>(ANIM::INTERVAL)]) %
					   animType[animName][static_cast<int>(ANIM::FRAME)];
}

void Player::Idle(const Input & p)
{
	if (p.IsPressing(PAD_INPUT_RIGHT) ||
		p.IsPressing(PAD_INPUT_LEFT))
	{
		updater = &Player::Run;
		SetAnim("run");
		turnFlag = (p.IsTrigger(PAD_INPUT_RIGHT) ? turnFlag = true 
												 : turnFlag = false);
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
			SetAnim("jump");
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
}

void Player::Run(const Input & p)
{
	if (p.IsPressing(PAD_INPUT_RIGHT))
	{
		SetAnim("run");
		turnFlag = true;
		vel.x = 1.0f;
	}
	else if (p.IsPressing(PAD_INPUT_LEFT))
	{
		SetAnim("run");
		turnFlag = false;
		vel.x = -1.0f;
	}
	else
	{
		SetAnim("idle");
		runFlag = false;
		vel.x = 0;
		updater = &Player::Idle;
	}

	if (groundFlag)
	{
		vel.y = 0;
		pos.y = groundLine - size.y;
		if (p.IsTrigger(PAD_INPUT_10))
		{
			SetAnim("jump");
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

}

void Player::Jump(const Input & p)
{
	if (groundFlag)
	{
		SetAnim("idle");
		updater = &Player::Idle;
		vel.y = 0;
		pos.y = groundLine - size.y;
	}
	else
	{
		vel.y += 0.7f;
	}

	if (p.IsPressing(PAD_INPUT_RIGHT))
	{
		turnFlag = true;
		vel.x = 1.0f;
	}
	else if (p.IsPressing(PAD_INPUT_LEFT))
	{
		turnFlag = false;
		vel.x = -1.0f;
	}
	else
	{
		SetAnim("jump");
		vel.x = 0;
	}
}

void Player::DebugDraw()
{
	if (animName == "run")
	{
		DrawString(0, 0, "走っている", 0xffffff);
	}
	else if (animName == "jump")
	{
		DrawString(0, 0, "飛んでいる", 0xffffff);
	}
	else if (animName == "idle")
	{
		DrawString(0, 0, "待機中", 0xffffff);
	}
	else
	{
		DrawString(0, 0, "アニメーションしてないやんけ", 0xffffff);
	}
}


void Player::Update(const Input & p)
{
	(this->*updater)(p);
	Anim();
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
	if (turnFlag)
	{
		DxLib::DrawGraph(pos.x, pos.y, LpImageMng.ImgGetID(fileName, divCnt, size)[chipCnt], turnFlag);
	}
	else
	{
		DxLib::DrawTurnGraph(pos.x, pos.y, LpImageMng.ImgGetID(fileName, divCnt, size)[chipCnt], turnFlag);
	}
	
}

