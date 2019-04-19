#include <DxLib.h>
#include "../Game.h"
#include "../CollisionDetector.h"
#include "../Input.h"
#include "Bubble.h"

Bubble::Bubble() : spitFrame(10), defSpeed(0.5f), colSpeed(1.5f)
{
	///	‰¼‚Ì‰Šú‰»
	Floating();
	nowCutIdx = 0;
	ReadActionFile("Action/bubble.act");		// –¼‘O‚Í‚±‚¤‚·‚é—\’è
	bubbleImage = DxLib::LoadGraph(actionData.imgFilePath.c_str());
	gameFlag = deleteFlag = false;

	vel.y = -(0.4f * (GetRand(6) + 1));
	invCnt  = spitFrame;
}

Bubble::Bubble(const bool& bubbleDir) : spitFrame(10), defSpeed(0.5f), colSpeed(1.5f)
{
	///	‰¼‚Ì‰Šú‰»
	Shot();
	nowCutIdx = 0;
	ReadActionFile("Action/bubble.act");		// –¼‘O‚Í‚±‚¤‚·‚é—\’è
	bubbleImage = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	this->bubbleDir = bubbleDir;
	gameFlag = true;
	deleteFlag = false;
	invCnt  = spitFrame;
}

Bubble::~Bubble()
{
	DxLib::DeleteGraph(bubbleImage);
}

const Vector2f & Bubble::GetPos()
{
	return pos;
}

Rect Bubble::GetRect()
{
	auto center = Vector2(pos.x + (size.x / 2), pos.y + (size.x / 2));
	auto rectSize = Size(size.x, size.y);

	return Rect(center, rectSize);
}

Rect Bubble::ShotGetRect()
{
	auto center = Vector2(pos.x + (size.x / 2), pos.y + (size.x / 2));
	auto rectSize = Size(size.x / 2, size.y / 2);

	return Rect(center, rectSize);
}

const bool& Bubble::CheckDelete()
{
	return deleteFlag;
}

void Bubble::HitAcross(const Rect & player, const Rect& wall)
{
	if (updater == &Bubble::FloatingUpdate)
	{
		SideCheck(player, wall);
	}
	
}

void Bubble::MoveContact(const Rect & rcB)
{
	auto hitCheck = (CollisionDetector::CollCheck(GetRect(), rcB));
	auto sideCheck = (CollisionDetector::SideCollCheck(GetRect(), rcB));

	/// –A“¯m‚ª“–‚½‚Á‚½‚Ì‹““®(‰¡ˆÚ“®)
	if (GetRect().Right() - (size.x / 3) > rcB.Left() &&
	    GetRect().Right() - (size.x / 3) < rcB.center.x && 
		sideCheck)
	{
		vel.x = -defSpeed / 2;
	}
	else if (GetRect().Left() + (size.x / 3) < rcB.Right() &&
			 GetRect().Left() + (size.x / 3) > rcB.center.x &&
			 sideCheck)
	{
		vel.x = defSpeed / 2;
	}
	else
	{
		vel.x = 0;
	}

	/// –A“¯m‚ª“–‚½‚Á‚½‚Ì‹““®(cˆÚ“®)
	if (GetRect().Bottom() > rcB.center.y &&
		GetRect().Bottom() < rcB.center.y + (size.y / 2) &&
		hitCheck)
	{
		vel.y = -defSpeed * 2;
	}
	else if (GetRect().Top() < rcB.center.y &&
			 GetRect().Top() > rcB.center.y - (size.y / 2) &&
			 hitCheck)
	{
		vel.y = defSpeed / 2;
	}
	else
	{
		vel.y = -defSpeed;
	}
	
}

int Bubble::HitPlayer(const Rect &rcB, const Input & p)
{
	if (updater == &Bubble::FloatingUpdate)
	{
		return GroundCheck(rcB, p);
	}
	return 0;
}

const bool& Bubble::HitEnemy(const Rect& rcA, const Rect& rcB)
{
	auto hitCheck = (CollisionDetector::CollCheck(rcA, rcB));
	if (updater == &Bubble::ShotUpdate && hitCheck)
	{
		deleteFlag = false;
		return true;
	}
	return false;
}

const bool& Bubble::HitObject(const Rect& rcA, const Rect& rcB)
{
	auto sideCheck = CollisionDetector::SideCollCheck(rcA, rcB);
	if (updater == &Bubble::ShotUpdate && sideCheck)
	{
		Floating();
		return true;
	}
	return false;
}

const bool& Bubble::HitBubble(const Rect& rcA, const Rect& rcB)
{
	auto hitCheck = CollisionDetector::CollCheck(rcA, rcB);

	if (updater == &Bubble::PopUpdate && hitCheck)
	{
		vel = Vector2f(0, 0);
		return true;
	}
	return false;
}

const bool& Bubble::CheckFloating()
{
	return (updater == &Bubble::FloatingUpdate);
}
void Bubble::ChangePop()
{
	if (updater != &Bubble::PopUpdate) { Pop(); }

}

void Bubble::SideCheck(const Rect & player, const Rect& wall)
{
	auto hitPlayer = (CollisionDetector::CollCheck(GetRect(), player));
	
	if (hitPlayer)
	{
		/// •Ç‚É“–‚½‚Á‚½A–A‚ğŠ„‚é
		if (CollisionDetector::SideCollCheck(GetRect(), wall))
		{
			Pop();
			return;
		}
		else
		{
			if (GetRect().center.x + (size.x / 2) > player.center.x - (player.size.width / 2) && 
				GetRect().center.x + (size.x / 2) < player.center.x)
			{
				/// ÌßÚ²Ô°‚Ì¶‘¤‚Æ“–‚½‚Á‚½‚Ì‹““®
				vel.x = -defSpeed * 2;
			}
			else if (GetRect().center.x - (size.x / 2) < player.center.x + (player.size.width / 2) && 
					 GetRect().center.x - (size.x / 2) > player.center.x)
			{
				/// ÌßÚ²Ô°‚Ì‰E‘¤‚Æ“–‚½‚Á‚½‚Ì‹““®
				vel.x = defSpeed * 2;
			}
			else{}
		}
	}
	else
	{
		vel.x = 0;
	}
}

int Bubble::GroundCheck(const Rect & rcB, const Input & p)
{
	auto hitCheck	 = (CollisionDetector::CollCheck(GetRect(), rcB));
	auto sideCheck   = (CollisionDetector::SideCollCheck(GetRect(), rcB));
	auto underBubble = (CollisionDetector::UnderCollCheck(GetRect(), rcB));		// –A‚Ì‰º‘¤‚Æ‚Ì“–‚½‚è”»’è
	auto underPlayer = (CollisionDetector::UnderCollCheck(rcB, GetRect()));		// ÌßÚ²Ô°‚Ì‰º‘¤‚Æ‚Ì“–‚½‚è”»’è

	if (hitCheck)
	{
		if (p.IsPressing(PAD_INPUT_1))
		{
			/// ’nã‚ÅÎŞÀİ‚ğ‰Ÿ‚µ‘±‚¯‚Ä‚¢‚é‚É–A‚É“–‚½‚é‚ÆA–A‚ªŠ„‚ê‚é
			if (underBubble || (GetRect().Top() < rcB.center.y + (size.y / 4)))
			{
				Pop();
				return 0;
			}
			/// ÎŞÀİ‚ğ‰Ÿ‚µ‘±‚¯‚Ä‚¢‚é‚ÆA–A‚Ìã‚ğ”ò‚Ô‚±‚Æ‚ª‚Å‚«‚é
			if (underPlayer)
			{
				return 1;
			}
		}
		else
		{
			if (underPlayer)
			{
				Pop();
				return 0;
			}
		}
		
	}
	return 0;
}

const bool& Bubble::CeilCheck()
{
	auto rtnFlag = false;
	if (pos.y < (size.y * 2) + (size.y / 2))
	{
		rtnFlag = true;

		if (pos.y < size.y + (size.y / 2))
		{
			vel.y = 0;
		}
	}
	return rtnFlag;
}

void Bubble::Shot()
{
	ChangeAction("shot");
	updater = &Bubble::ShotUpdate;
}

void Bubble::Floating()
{
	vel.x = 0;
	vel.y = -0.5;
	ChangeAction("floating");
	updater = &Bubble::FloatingUpdate;
}

void Bubble::Pop()
{
	ChangeAction("pop");
	turnFlag = false;				/// ´Ìª¸Ä‚Ì•`‰æ”½“]‚ğ–h‚¢‚Ä‚¢‚é
	updater = &Bubble::PopUpdate;
}

void Bubble::ShotUpdate()
{
	if (invCnt < 0)
	{
		Floating();
		vel.x = 0;
	}
	else
	{
		invCnt--;
		if (bubbleDir)
		{
			vel.x = -20.0;
		}
		else
		{
			vel.x = 20.0;
		}
	}
}

void Bubble::FloatingUpdate()
{
}

void Bubble::PopUpdate()
{
	vel = Vector2f(0, 0);
	deleteFlag = ProceedAnimFile();
}

void Bubble::Update()
{
	(this->*updater)();

	pos += vel;
}

void Bubble::Draw()
{
#ifdef _DEBUG
	if (gameFlag)
	DebugDraw();
#endif

	CharactorObject::Draw(bubbleImage);
}

void Bubble::DebugDraw()
{
	DrawString(100, 0, nowActionName.c_str(), 0xffffff);
	if (nowActionName == "floating")
	{
		DrawBox(GetRect().Left(), GetRect().Top(), GetRect().Right(), GetRect().Bottom(), 0xff0000, false);
	}
	else if (nowActionName == "shot")
	{
		DrawBox(ShotGetRect().Left(), ShotGetRect().Top(), ShotGetRect().Right(), ShotGetRect().Bottom(), 0xff0000, false);
	}
	else {}

}
