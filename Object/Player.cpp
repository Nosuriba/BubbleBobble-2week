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

	jumpFlag = groundFlag = dieFlag = false;
	invCnt	 = 0;
}

Player::Player(int groundLine)
{
	pos		 = vel  = Vector2f(0, 0);
	divCnt	 = size = Vector2(0, 0);
	fileName = "";
	this->groundLine = groundLine;

	jumpFlag = groundFlag = dieFlag = false;
	invCnt	 = 0;
}

Player::~Player()
{
}

void Player::InitAnim(void)
{
	AddAnim("idle", Vector2(0, 0), 3, 30);
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

void Player::Move(const Input & p)
{
	if (p.IsPressing(PAD_INPUT_RIGHT))
	{
		runFlag = true;
		turnFlag = true;
		vel.x = 1.0f;
	}
	else if (p.IsPressing(PAD_INPUT_LEFT))
	{
		runFlag = true;
		turnFlag = false;
		vel.x = -1.0f;
	}
	else
	{
		runFlag = false;
		vel.x = 0;
	}

}

void Player::Jump(const Input & p)
{
	if (!jumpFlag)
	{
		if (p.IsTrigger(PAD_INPUT_10))
		{
 			jumpFlag = true;
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
		vel.y += 0.7f;
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
	// (this->*updater)(p);
	Move(p);
	Jump(p);
	Fall();
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
	DxLib::DrawTurnGraph(pos.x, pos.y, LpImageMng.ImgGetID(fileName, divCnt, size)[chipCnt], turnFlag);
}

