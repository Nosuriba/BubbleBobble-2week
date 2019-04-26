#include <DxLib.h>
#include "../Game.h"
#include "../CollisionDetector.h"
#include "../Input.h"
#include "../AudioMng.h"
#include "Bubble.h"

Bubble::Bubble() : spitFrame(10), defSpeed(0.5f), colSpeed(1.5f)
{
	///	‰¼‚Ì‰Šú‰»
	Floating();
	nowCutIdx   = 0;
	ReadActionFile("Action/bubble.act");		// –¼‘O‚Í‚±‚¤‚·‚é—\’è
	bubbleImage = DxLib::LoadGraph(actionData.imgFilePath.c_str());
	gameFlag    = deleteFlag = false;
	vel.y	    = -(0.4f * (GetRand(6) + 1));
	invCnt      = spitFrame;
}

Bubble::Bubble(const bool& bubbleDir) : spitFrame(10), defSpeed(0.5f), colSpeed(1.5f)
{
	///	‰¼‚Ì‰Šú‰»
	Shot();
	nowCutIdx = 0;
	ReadActionFile("Action/bubble.act");		// –¼‘O‚Í‚±‚¤‚·‚é—\’è
	bubbleImage = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	this->bubbleDir = bubbleDir;
	gameFlag	= true;
	deleteFlag  = false;
	invCnt		= spitFrame;
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
	auto center   = Vector2(pos.x + (size.x / 2), pos.y + (size.x / 2));
	auto rectSize = Size(size.x, size.y);

	return Rect(center, rectSize);
}

Rect Bubble::ShotGetRect()
{
	auto center   = Vector2(pos.x + (size.x / 2), pos.y + (size.x / 2));
	auto rectSize = Size(size.x / 2, size.y / 2);

	return Rect(center, rectSize);
}

bool Bubble::CheckDelete()
{
	return deleteFlag;
}

void Bubble::HitAcross(const Rect & pRect, const Rect& wRect)
{
	if (updater == &Bubble::FloatingUpdate)
	{
		SideCheck(pRect, wRect);
	}
}

void Bubble::MoveContact(const Rect & bblRect)
{
	auto hitCheck  = (CollisionDetector::CollCheck(GetRect(), bblRect));
	auto sideCheck = (CollisionDetector::SideCollCheck(GetRect(), bblRect));

	if (updater == &Bubble::FloatingUpdate)
	{
		/// –A“¯m‚ª“–‚½‚Á‚½‚Ì‹““®(‰¡ˆÚ“®)
		if (GetRect().Right() - (size.x / 3) > bblRect.Left() &&
			GetRect().Right() - (size.x / 3) < bblRect.center.x &&
			sideCheck)
		{
			vel.x = -defSpeed;
		}
		else if (GetRect().Left() + (size.x / 3) < bblRect.Right() &&
				 GetRect().Left() + (size.x / 3) > bblRect.center.x &&
				 sideCheck)
		{
			vel.x = defSpeed;
		}
		else
		{
			vel.x = 0;
		}

		/// –A“¯m‚ª“–‚½‚Á‚½‚Ì‹““®(cˆÚ“®)
		if (GetRect().Bottom() > bblRect.center.y &&
			GetRect().Bottom() < bblRect.center.y + (size.y / 3) &&
			hitCheck)
		{
			vel.y = -defSpeed * 2;
			return;
		}
		else if (GetRect().Top() < bblRect.center.y &&
				 GetRect().Top() > bblRect.center.y - (size.y / 3) &&
				 hitCheck)
		{
			vel.y = defSpeed;
			return;
		}
		else
		{
			vel.y = -defSpeed;
		}
	}
}

bool Bubble::HitPlayer(const Rect &pRect, const Input & p)
{
	if (updater == &Bubble::FloatingUpdate)
	{
		return UnderCheck(pRect, p);
	}
	return false;
}

bool Bubble::HitEnemy(const Rect& eRect)
{
	auto hitCheck = (CollisionDetector::CollCheck(GetRect(), eRect));
	if (updater == &Bubble::ShotUpdate && hitCheck)
	{
		deleteFlag = false;
		return true;
	}
	return false;
}

bool Bubble::HitObject(const Rect& objRect)
{
	auto selHitCheck = [&](const Rect& objRect)		
	{
		if (objRect.size.height == size.y)
		{
			/// •Ç‚Æ‚Ì“–‚½‚è”»’è
			return CollisionDetector::SideCollCheck(GetRect(), objRect);
		}
		else
		{
			/// ÌŞÛ¯¸‚Æ‚Ì“–‚½‚è”»’è
			return CollisionDetector::SideCollCheck(ShotGetRect(), objRect);
		}
	};
	
	if (updater == &Bubble::ShotUpdate && selHitCheck(objRect))
	{
		Floating();
		return true;
	}
	return false;
}

bool Bubble::HitBubble(const Rect& bblRect)
{
	auto hitCheck = CollisionDetector::CollCheck(GetRect(), bblRect);

	if (updater == &Bubble::PopUpdate && hitCheck)
	{
		vel = Vector2f(0, 0);
		return true;
	}
	return false;
}

void Bubble::ChangePop()
{

	if (updater != &Bubble::PopUpdate)
	{ 
		AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().pop);
		AudioMng::GetInstance().ChangeVolume(200, AudioMng::GetInstance().GetSound().pop);

		Pop(); 
	}
}

void Bubble::SideCheck(const Rect & pRect, const Rect& wRect)
{
	auto hitCheck  = (CollisionDetector::CollCheck(GetRect(), pRect));
	auto sideCheck = (CollisionDetector::SideCollCheck(GetRect(), wRect));
	
	if (hitCheck)
	{
		/// •Ç‚É“–‚½‚Á‚½A–A‚ğŠ„‚é
		if (sideCheck)
		{
			AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().pop);
			AudioMng::GetInstance().ChangeVolume(200, AudioMng::GetInstance().GetSound().pop);
			Pop();
			return;
		}
		else
		{
			if (GetRect().center.x + (size.x / 2) > pRect.center.x - (pRect.size.width / 2) &&
				GetRect().center.x + (size.x / 2) < pRect.center.x)
			{
				/// ÌßÚ²Ô°‚Ì¶‘¤‚Æ“–‚½‚Á‚½‚Ì‹““®
				vel.x = -charSpeed;
			}
			else if (GetRect().center.x - (size.x / 2) < pRect.center.x + (pRect.size.width / 2) &&
					 GetRect().center.x - (size.x / 2) > pRect.center.x)
			{
				/// ÌßÚ²Ô°‚Ì‰E‘¤‚Æ“–‚½‚Á‚½‚Ì‹““®
				vel.x = charSpeed;
			}
			else {}
		}
	}
	else
	{
		vel.x = 0;
	}
}

bool Bubble::UnderCheck(const Rect & pRect, const Input & p)
{
	auto hitCheck	 = (CollisionDetector::CollCheck(GetRect(), pRect));
	auto underBubble = (CollisionDetector::UnderCollCheck(GetRect(), pRect));		// –A‚Ì‰º‘¤‚Æ‚Ì“–‚½‚è”»’è
	auto underPlayer = (CollisionDetector::UnderCollCheck(pRect, GetRect()));		// ÌßÚ²Ô°‚Ì‰º‘¤‚Æ‚Ì“–‚½‚è”»’è

	if (hitCheck)
	{
		if (p.IsPressing(PAD_INPUT_1))
		{
			/// ’nã‚ÅÎŞÀİ‚ğ‰Ÿ‚µ‘±‚¯‚Ä‚¢‚é‚É–A‚É“–‚½‚é‚ÆA–A‚ªŠ„‚ê‚é
			if (underBubble || (GetRect().Top() < pRect.center.y + (size.y / 4)))
			{
				AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().pop);
				AudioMng::GetInstance().ChangeVolume(200, AudioMng::GetInstance().GetSound().pop);
				Pop();
				return false;
			}
			/// ÎŞÀİ‚ğ‰Ÿ‚µ‘±‚¯‚Ä‚¢‚é‚ÆA–A‚Ìã‚ğ”ò‚Ô‚±‚Æ‚ª‚Å‚«‚é
			if (underPlayer)
			{
				return true;
			}
		}
		else
		{
			if (underPlayer)
			{
				AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().pop);
				AudioMng::GetInstance().ChangeVolume(200, AudioMng::GetInstance().GetSound().pop);
				Pop();
				return false;
			}
		}
		
	}
	return false;
}

bool Bubble::CeilCheck()
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
	vel.y = -0.5;					/// ‚Æ‚è‚ ‚¦‚¸A‰¼İ’è(‹C—¬—p‚ÌƒNƒ‰ƒXÀ‘•‚Ü‚Å)
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
