#include <DxLib.h>
#include "Enemy.h"
#include "../Game.h"
#include "../CollisionDetector.h"

Enemy::Enemy() : fallAccel(0.5f)
{
	Run();
	nowCutIdx = 0;
	ReadActionFile("Action/enemy.act");
	enemyImg = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	pos  = vel = Vector2f(0, 0);
	size = Vector2(0,0);
}

Enemy::Enemy(int groundLine) : fallAccel(0.5f)
{
	Run();
	nowCutIdx = 0;
	ReadActionFile("Action/enemy.act");
	enemyImg = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	this->groundLine = groundLine;
	pos  = vel = Vector2f(0, 0);
	jumpFlag = dieFlag = turnFlag = false;
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

const bool & Enemy::GetDieFlag()
{
	return dieFlag;
}

bool Enemy::HitPlayer(const Rect & pRect)
{
	auto hitCheck  = CollisionDetector::CollCheck(GetRect(), pRect);
	auto sideCheck = CollisionDetector::SideCollCheck(GetRect(), pRect);

	if (updater == &Enemy::BubbleUpdate && hitCheck)
	{
		if (!sideCheck)
		{
			/// �ЂƂ܂��u���Ƃ�
		}

		if (pRect.Right() > GetRect().Left() && 
			pRect.Right() < GetRect().center.x)
		{
			vel.x = charSpeed;
			vel.y = -12.f;
		}
		else if (pRect.Left() < GetRect().Right() &&
				 pRect.Left() > GetRect().center.x)
		{
			vel.x = -charSpeed;
			vel.y = -12.f;
		}
		else
		{

		}
		Die();
	}
	
	return false;
}

bool Enemy::HitBubble(const Rect & bRect)
{
	auto hitCheck = CollisionDetector::CollCheck(GetRect(), bRect);

	if (updater != &Enemy::BubbleUpdate && 
		updater != &Enemy::DieUpdate	&& hitCheck)
	{
		Bubble();
		return true;
	}
	return false;
}

bool Enemy::HitWall(const Rect & wRect)
{
	auto hitCheck = CollisionDetector::SideCollCheck(GetRect(), wRect);
	if (hitCheck && updater != &Enemy::DieUpdate)
	{
		/// �ǂɓ���������A�����]������悤�ɂ��Ă���
		turnFlag = !turnFlag;
		pos.x	 = (turnFlag ? wRect.Right() : wRect.Left() - size.x);
		return true;
	}

	return false;
}

bool Enemy::HitGround(const Rect & bRect)
{
	auto underCheck = CollisionDetector::UnderCollCheck(GetRect(), bRect);
	
	if (updater != &Enemy::DieUpdate)
	{
		/// �������Ƀu���b�N�̏�ɏ�������̏���
		if (underCheck && vel.y >= 0.f && GetRect().Bottom() > (size.y + bRect.size.height))
		{
			this->vel.y = 0;
			this->groundLine = bRect.Top() + 1;		/// ���ɏ����߂荞�ނ悤�ɂ��Ă���B
			return true;
		}
		this->groundLine = Game::GetInstance().GetScreenSize().y + (size.y * 2);
	}
	return false;
}

bool Enemy::UpperCheck(const Rect& pRect, const Rect & bRect)
{
	return false;
}

void Enemy::DieControl(const Rect& objRect)
{
	auto selHitCheck = [=](const Rect& objRect)
	{
		if (objRect.size.height == size.y)
		{
			/// �ǂƂ̓����蔻��
			return CollisionDetector::CollCheck(GetRect(), objRect);
		}
		else
		{
			/// ��ۯ��Ƃ̓����蔻��
			return CollisionDetector::UnderCollCheck(GetRect(), objRect);
		}
	};
	/// ��޼ު�Ă̻��ފm�F�p
	auto sizeCheck = (size.y == objRect.size.height ? true : false);

	if (updater == &Enemy::DieUpdate)
	{
		if (selHitCheck(objRect) && !sizeCheck && vel.y == charSpeed)
		{
			/*vel		= Vector2f(0, 0);
			dieFlag = true;*/
			return;
		}

		/// �ǂɓ�����Ȃ��Ƃ���������
		if (selHitCheck(objRect) && sizeCheck)
		{
			vel.x = (vel.x == charSpeed ? -charSpeed : charSpeed);
		}
	}
}

void Enemy::Idle()
{
	if (nowActionName != "run") { ChangeAction("run"); }
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
		Jump();
		return;
	}
	auto changeTurn = ((waitCnt / 20) % 2 ? true : false);
	turnFlag = changeTurn;
	
	ProceedAnimFile();
}

void Enemy::RunUpdate()
{
	if (turnFlag)
	{
		vel.x = charSpeed;
	}
	else
	{
		vel.x = -charSpeed;
	}


	ProceedAnimFile();
}

void Enemy::JumpUpdate()
{
	ProceedAnimFile();
}

void Enemy::BubbleUpdate()
{
	vel.y = -0.5;		/// �Ƃ肠�����A���ݒ�
	ProceedAnimFile();
}

void Enemy::DieUpdate()
{
	if (!dieFlag)	/// ��ŊO��
	{
		if (vel.y < charSpeed)
		{
			vel.y += 0.5f;
		}
		else
		{
			vel.x = 0;
			vel.y = charSpeed;
		}

	}
	
	ProceedAnimFile();
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