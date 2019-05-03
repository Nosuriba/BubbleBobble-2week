#include <DxLib.h>
#include "Enemy.h"
#include "../Game.h"
#include "../Input.h"
#include "../CollisionDetector.h"
#include "../AudioMng.h"

Enemy::Enemy() : fallAccel(0.3f)
{
}

Enemy::Enemy(int groundLine) : fallAccel(0.3f)
{
	Run();
	nowCutIdx = 0;
	ReadActionFile("Action/enemy.act");
	enemyImg = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	this->groundLine = groundLine;
	pos  = vel = Vector2f(0, 0);
	jumpFlag = dieFlag = turnFlag = riseFlag = false;
	size = Vector2(0, 0);


	updater = &Enemy::RunUpdate;
}

Enemy::~Enemy()
{
	DxLib::DeleteGraph(enemyImg);
}

Rect Enemy::GetRect()
{
	auto center = Vector2(pos.x + (size.x / 2), pos.y + (size.x / 2));
	auto rectSize = Size(size.x, size.y);

	return Rect(center, rectSize);
}
bool Enemy::CheckAlive()
{
	/// “G‚ª¶‘¶ó‘Ô‚©‚Ì”»’è—p
	return (updater != &Enemy::BubbleUpdate && updater != &Enemy::DieUpdate);
}

bool Enemy::HitPlayer(const Rect & pRect, const Rect& wRect, const Input& p)
{
	SideCheck(pRect, wRect);		// “G‚Ì‰¡‘¤‚É“–‚½‚Á‚½‚©’²‚×‚Ä‚¢‚é
	return UnderCheck(pRect, p);	// 
}

bool Enemy::HitBubble(const Rect & bRect, const bool& bblCheck)
{
	auto hitCheck = CollisionDetector::CollCheck(GetRect(), bRect);

	/// ¼®¯Ä‚Ìó‘Ô‚Ì–A‚É“–‚½‚Á‚½A“G‚ª–Aó‘Ô‚É‚È‚é
	if (updater != &Enemy::BubbleUpdate && 
		updater != &Enemy::DieUpdate	&& 
		hitCheck && bblCheck)
	{
		Bubble();
		return true;
	}
	return false;
}

bool Enemy::HitWall(const Rect & wRect)
{
	auto hitCheck = CollisionDetector::CollCheck(GetRect(), wRect);
	if (hitCheck && updater != &Enemy::DieUpdate)
	{
		/// •Ç‚É“–‚½‚Á‚½‚çA•ûŒü“]Š·‚·‚é‚æ‚¤‚É‚µ‚Ä‚¢‚é
		turnFlag = !turnFlag;
		vel.x	 = (vel.x == charSpeed ? -charSpeed : charSpeed);
		return true;
	}

	return false;
}

bool Enemy::HitGround(const Rect& pRect, const Rect & bRect)
{
	auto underCheck = CollisionDetector::UnderCollCheck(GetRect(), bRect);
	
	if (updater == &Enemy::RunUpdate)
	{
		if (!riseFlag)
		{
			/// —‰º’†‚ÉƒuƒƒbƒN‚Ìã‚Éæ‚Á‚½‚Ìˆ—
			if (underCheck)
			{
				vel.y = 0;
				groundLine = bRect.Top() + 1;		
				return true;
			}
			vel.y = 5.0f;
			groundLine = Game::GetInstance().GetScreenSize().y + (size.y * 2);
		}
	}
	return false;
}

bool Enemy::UpperCheck(const Rect& pRect, const Rect & bRect)
{
	auto rtnFlag = false;

	/// ÌßÚ²Ô°‚ª“G‚æ‚èã‚É‚¢‚ÄA“ªã‚ÉƒuƒƒbƒN‚ª‚ ‚Á‚½‚ÉÌßÚ²Ô°‚ğ’T‚·‹““®‚É‚·‚é
	if (updater == &Enemy::RunUpdate)
	{
		if (pos.y > pRect.Bottom() &&
	   (int)pos.x == (int)bRect.center.x)		/// ‚Ü‚¾ã¸‚·‚éˆ—‚ªŠ®‘S‚É‚Å‚«‚Ä‚¢‚È‚¢
		{
			turnFlag   = (GetRect().center.x < pRect.center.x ? true : false);
			groundLine = 0;
			riseFlag   = true;
			Idle();
		}
		return false;
	}

	/// ÌßÚ²Ô°‚ğˆê’èŠÔ’T‚µ‚Ä‚©‚çã¸‚·‚é‚æ‚¤‚É‚µ‚Ä‚¢‚é
	if (updater == &Enemy::IdleUpdate && waitCnt < 0)
	{
		if (riseFlag)
		{
			vel.y = -charSpeed;
			if (RiseCheck(bRect))
			{
				/// ã¸’†‚É“–‚½‚Á‚½ÌŞÛ¯¸‚Ìã‘¤‚ğ’n–Ê‚É‚µ‚Ä‚¢‚é
				turnFlag	= (GetRect().center.x > pRect.center.x ? false : true);
				groundLine  = bRect.Top() + 1;
				riseFlag    = false;
				rtnFlag		= true;
			}
		}
		else
		{
			/// “G‚Ì‰º‘¤‚ªA’n–Ê‚É“–‚½‚Á‚½‚ç—‰º‚ğ~‚ß‚é
			if ((int)GetRect().Bottom() <= groundLine)
			{
				vel.y = 0;
				Run();
			}
		}
		
	}
	return rtnFlag;
}

void Enemy::DieControl(const Rect& objRect)
{
	auto selHitCheck = [&](const Rect& objRect)
	{
		if (objRect.size.height == size.y)
		{
			/// •Ç‚Æ‚Ì“–‚½‚è”»’è
			return CollisionDetector::CollCheck(GetRect(), objRect);
		}
		else
		{
			/// ÌŞÛ¯¸‚Æ‚Ì“–‚½‚è”»’è
			return CollisionDetector::UnderCollCheck(GetRect(), objRect);
		}
	};
	/// µÌŞ¼Şª¸Ä‚Ì»²½ŞŠm”F—p
	auto sizeCheck = (size.y == objRect.size.height ? true : false);

	if (updater == &Enemy::DieUpdate)
	{
		if (selHitCheck(objRect) && !sizeCheck && vel.y == charSpeed)
		{
			/// °‚É“–‚½‚Á‚½A€–Só‘Ô‚É‚·‚é(Œã‚ÅA“G‚ªÁ‚¦‚é‚æ‚¤‚É‚·‚é)
			vel		= Vector2f(0, 0);
			dieFlag = true;
			return;
		}

		if (selHitCheck(objRect) && sizeCheck)
		{
			/// •Ç‚É“–‚½‚Á‚½‚É“G‚ª’µ‚Ë•Ô‚é‚æ‚¤‚É‚µ‚Ä‚¢‚é
			pos.x = (vel.x == charSpeed ? objRect.Left() - size.x : objRect.Right());	
			vel.x = (vel.x == charSpeed ? -charSpeed : charSpeed);
		}
	}
}

void Enemy::Idle()
{
	if (nowActionName != "run") { ChangeAction("run"); }
	waitCnt = 60;
	vel = Vector2f(0, 0);
	updater = &Enemy::IdleUpdate;
}

void Enemy::Run()
{
	if (nowActionName != "run") { ChangeAction("run"); }
	updater = &Enemy::RunUpdate;
}

void Enemy::Jump()
{
	if (nowActionName != "run") { ChangeAction("run"); }
	updater = &Enemy::JumpUpdate;
}

void Enemy::Bubble()
{
	ChangeAction("bubble");
	vel.x = 0.f;
	updater = &Enemy::BubbleUpdate;
}

void Enemy::Die()
{
	ChangeAction("die");
	updater = &Enemy::DieUpdate;
}

void Enemy::IdleUpdate()
{
	waitCnt--;
	if (waitCnt < 0)
	{
		//Jump();
		return;
	}
	auto changeTurn = ((waitCnt / 20) % 2 ? true : false);
	turnFlag = changeTurn;
	
	ProceedAnimFile();
}

void Enemy::RunUpdate()
{
	if (vel.y == 0.f)
	{
		if (turnFlag)
		{
			vel.x = charSpeed;
		}
		else
		{
			vel.x = -charSpeed;
		}
	}
	

	ProceedAnimFile();
}

void Enemy::JumpUpdate()
{
	ProceedAnimFile();
}

void Enemy::BubbleUpdate()
{
	vel.y = -0.5;		/// ‚Æ‚è‚ ‚¦‚¸A‰¼İ’è
	ProceedAnimFile();
}

void Enemy::DieUpdate()
{
	if (!dieFlag)
	{
		if (vel.y < charSpeed)
		{
			vel.y += fallAccel;
		}
		else
		{
			vel.x = 0;
			vel.y = charSpeed;
		}
	}
	
	
	ProceedAnimFile();
}

bool Enemy::RiseCheck(const Rect& bRect)
{
	auto underBlock = CollisionDetector::UnderCollCheck(bRect, GetRect());
	if (underBlock)
	{
		return true;
	}
	return false;
}

void Enemy::SideCheck(const Rect & pRect, const Rect & wRect)
{
	auto hitCheck = CollisionDetector::CollCheck(GetRect(), pRect);
	auto wSideCheck = CollisionDetector::SideCollCheck(GetRect(), wRect);

	if (updater == &Enemy::BubbleUpdate &&
		updater != &Enemy::DieUpdate &&
		hitCheck)
	{
		if (wSideCheck)
		{
			if (wRect.Right() > GetRect().Left())
			{
				AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
				Die();
				turnFlag = true;
				vel.x = charSpeed;
				vel.y = -12.f;
			}
			else if (wRect.Left() < GetRect().Right())
			{
				AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
				Die();
				turnFlag = false;
				vel.x = -charSpeed;
				vel.y = -12.f;
			}
			else {}
		}
		if (pRect.Right() > GetRect().Left() &&
			pRect.Right() < GetRect().center.x)
		{
			vel.x = charSpeed;
		}
		else if (pRect.Left() < GetRect().Right() &&
				 pRect.Left() > GetRect().center.x)
		{
			vel.x = -charSpeed;
		}
		else {}
	}
	else
	{
		if (updater != &Enemy::DieUpdate) { vel.x = 0.f; }
	}
}

bool Enemy::UnderCheck(const Rect & pRect, const Input & p)
{
	auto hitDir		 = (pRect.Right() < GetRect().center.x);						// ÌßÚ²Ô°‚Ì“–‚½‚Á‚½•ûŒü
	auto hitCheck	 = (CollisionDetector::CollCheck(GetRect(), pRect));
	auto underBubble = (CollisionDetector::UnderCollCheck(GetRect(), pRect));		// –A‚Ì‰º‘¤‚Æ‚Ì“–‚½‚è”»’è
	auto underPlayer = (CollisionDetector::UnderCollCheck(pRect, GetRect()));		// ÌßÚ²Ô°‚Ì‰º‘¤‚Æ‚Ì“–‚½‚è”»’è

	if (hitCheck && updater == &Enemy::BubbleUpdate)
	{
		/// ¼Ş¬İÌßÎŞÀİ‚ğ‰Ÿ‚µ‚Á‚Ï‚È‚µ‚Ì
		if (p.IsPressing(PAD_INPUT_1))
		{
			/// ÌßÚ²Ô°‚ª’nã‚Å–Aó‘Ô‚Ì“G‚É“–‚½‚é‚ÆB–A‚ªŠ„‚ê‚é
			if (underBubble || (GetRect().Top() < pRect.center.y + (size.y / 4)))
			{
				if (hitDir)
				{
					AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
					Die();
					turnFlag = true;
					vel.x = charSpeed;
					vel.y = -12.f;
				}
				else
				{
					AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
					Die();
					turnFlag = false;
					vel.x = -charSpeed;
					vel.y = -12.f;
				}
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
			/// ¼Ş¬İÌßÎŞÀİ‚ğ‰Ÿ‚µ‚Ä‚¢‚È‚¢ó‘Ô‚Ì
			if (underPlayer)
			{
				if (hitDir)
				{
					AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
					Die();
					turnFlag = true;
					vel.x = charSpeed;
					vel.y = -12.f;
				}
				else
				{
					AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
					Die();
					turnFlag = false;
					vel.x = -charSpeed;
					vel.y = -12.f;
				}
				return false;
			}
		}
	}
	return false;
}

bool Enemy::OnGround()
{
	if (pos.y + size.y >= groundLine)
	{
		jumpFlag = false;
		return true;
	}
	return false;
}

void Enemy::Update()
{
	(this->*updater)();

	pos += vel;
}

void Enemy::Draw()
{
	CharactorObject::Draw(enemyImg);

#ifdef _DEBUG
	DebugDraw();
#endif 

}

void Enemy::DebugDraw()
{
	DrawString(0, 0, nowActionName.c_str(), 0xffffff);
	DrawBox(GetRect().Left(), GetRect().Top(), GetRect().Right(), GetRect().Bottom(), 0xff0000, false);
}