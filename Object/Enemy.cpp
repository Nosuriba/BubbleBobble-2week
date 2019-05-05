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
	/// �G��������Ԃ��̔���p
	return (updater != &Enemy::BubbleUpdate && updater != &Enemy::DieUpdate);
}

bool Enemy::HitPlayer(const Rect & pRect, const Rect& wRect, const Input& p)
{
	SideCheck(pRect, wRect);		// �G�̉����ɓ������������ׂĂ���
	return UnderCheck(pRect, p);	// 
}

bool Enemy::HitBubble(const Rect & bRect, const bool& bblCheck)
{
	auto hitCheck = CollisionDetector::CollCheck(GetRect(), bRect);

	/// ���Ă̏�Ԃ̖A�ɓ����������A�G���A��ԂɂȂ�
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
		/// �ǂɓ���������A�����]������悤�ɂ��Ă���
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
			/// �������Ƀu���b�N�̏�ɏ�������̏���
			if (underCheck && GetRect().Bottom() > size.y * 3)
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
	/// ��ڲ԰���G����ɂ��āA����Ƀu���b�N��������������ڲ԰��T�������ɂ���
	if (updater == &Enemy::RunUpdate)
	{
		if (pos.y > pRect.Bottom() &&
	   (int)pos.x == (int)bRect.center.x &&
			bRect.Bottom() > size.y * 3 && 
			bRect.Bottom() < pos.y)
		{
			turnFlag   = (GetRect().center.x < pRect.center.x ? true : false);
			groundLine = 0;
			riseFlag   = true;
			Idle();
		}
		return false;
	}

	/// ��ڲ԰����莞�ԒT���Ă���㏸����悤�ɂ��Ă���
	if (updater == &Enemy::IdleUpdate && waitCnt < 0)
	{
		if (riseFlag)
		{
			vel.y = -charSpeed;
			if (RiseCheck(bRect))
			{
				/// �㏸���ɓ���������ۯ��̏㑤��n�ʂɂ��Ă���
				turnFlag	= (GetRect().center.x > pRect.center.x ? false : true);
				groundLine  = bRect.Top() + 1;
				riseFlag    = false;
				return true;
			}
		}
		else
		{
			/// �G�̉������A�n�ʂɓ��������痎�����~�߂�
			if ((int)GetRect().Bottom() <= groundLine)
			{
				vel.y = 0;
				Run();
			}
		}
		
	}
	return false;
}

void Enemy::DieControl(const Rect& objRect)
{
	auto selHitCheck = [&](const Rect& objRect)
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
			/// ���ɓ����������A���S��Ԃɂ���(��ŁA�G��������悤�ɂ���)
			vel		= Vector2f(0, 0);
			dieFlag = true;
			return;
		}

		if (selHitCheck(objRect) && sizeCheck)
		{
			/// �ǂɓ����������ɓG�����˕Ԃ�悤�ɂ��Ă���
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
	vel.y = -0.5;		/// �Ƃ肠�����A���ݒ�
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
	auto hitCheck   = CollisionDetector::CollCheck(GetRect(), pRect);
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
	auto hitDir		 = (pRect.Right() < GetRect().center.x);						// ��ڲ԰�̓�����������
	auto hitCheck	 = (CollisionDetector::CollCheck(GetRect(), pRect));
	auto underBubble = (CollisionDetector::UnderCollCheck(GetRect(), pRect));		// �A�̉����Ƃ̓����蔻��
	auto underPlayer = (CollisionDetector::UnderCollCheck(pRect, GetRect()));		// ��ڲ԰�̉����Ƃ̓����蔻��

	if (hitCheck && updater == &Enemy::BubbleUpdate)
	{
		/// �ެ������݂��������ςȂ��̎�
		if (p.IsPressing(PAD_INPUT_1))
		{
			/// ��ڲ԰���n��ŖA��Ԃ̓G�ɓ�����ƁB�A�������
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
			/// ���݂����������Ă���ƁA�A�̏���Ԃ��Ƃ��ł���
			if (underPlayer)
			{
				return true;
			}
		}
		else
		{
			/// �ެ������݂������Ă��Ȃ���Ԃ̎�
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