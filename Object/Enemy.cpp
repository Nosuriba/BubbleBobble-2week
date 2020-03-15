#include <DxLib.h>
#include "Enemy.h"
#include "../Game.h"
#include "../Input.h"
#include "../CollisionDetector.h"
#include "../AudioMng.h"

const float fallAccel = 0.3f;

Enemy::Enemy()
{
}

Enemy::Enemy(int groundLine)
{
	Run();

	/// �摜ID�̏�����S
	nowCutIdx = 0;
	ReadActionFile("Action/enemy.act");
	enemyImg = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	/// ���̏�����
	this->groundLine = groundLine;

	/// ���W�Ƒ��x�̏�����
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
bool Enemy::IsAlive()
{
	/// �G��������Ԃ��̔���p
	return (updater != &Enemy::BubbleUpdate && updater != &Enemy::DieUpdate);
}

bool Enemy::HitPlayer(const Rect & pRect, const Rect& wRect, const Input& p)
{
	IsSide(pRect, wRect);		// �G�̉����ɓ������������ׂĂ���
	return IsUnder(pRect, p);	// 
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
	auto isHitWall = CollisionDetector::CollCheck(GetRect(), wRect);
	if (isHitWall && updater != &Enemy::DieUpdate)
	{
		/// �G�̕����]��
		turnFlag = !turnFlag;
		vel.x	 = (vel.x == defSpeed ? -defSpeed : defSpeed);
		return true;
	}

	return false;
}

bool Enemy::HitGround(const Rect& pRect, const Rect & bRect)
{
	auto isUnder = CollisionDetector::UnderCollCheck(GetRect(), bRect);
	
	if (updater == &Enemy::RunUpdate)
	{
		if (!riseFlag)
		{
			/// �������Ƀu���b�N�̏�ɏ�������̏���
			if (isUnder && GetRect().Bottom() > size.y * 3)
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
	if (updater == &Enemy::RunUpdate)
	{
		/// ����̃v���C���[��T�����鏈��
		if (pos.y > pRect.Bottom()				&&
			(int)pos.x == (int)bRect.center.x	&&
			bRect.Bottom() > size.y * 3			&& 
			bRect.Bottom() < pos.y)
		{
			turnFlag   = (GetRect().center.x < pRect.center.x ? true : false);
			groundLine = 0;
			riseFlag   = true;
			Idle();
		}
		return false;
	}

	/// �v���C���[�����������A�G���㏸������
	if (updater == &Enemy::IdleUpdate && waitCnt < 0)
	{
		if (riseFlag)
		{
			vel.y = -defSpeed;
			if (IsRiseUnder(bRect))
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
	/// �G�̃T�C�Y�m�F�p
	auto isEnemySize = (size.y == objRect.size.height ? true : false);

	if (updater == &Enemy::DieUpdate)
	{
		if (selHitCheck(objRect) && !isEnemySize && vel.y == defSpeed)
		{
			/// ���ɓ����������A���S��Ԃɂ���
			vel		= Vector2f(0, 0);
			dieFlag = true;
			return;
		}

		if (selHitCheck(objRect) && isEnemySize)
		{
			/// �ǂɓ����������ɓG�𔽎˂����Ă���
			pos.x = (vel.x == defSpeed ? objRect.Left() - size.x : objRect.Right());	
			vel.x = (vel.x == defSpeed ? -defSpeed : defSpeed);
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
		return;
	}
	turnFlag = ((waitCnt / 20) % 2 ? true : false);
	
	ProceedAnimFile();
}

void Enemy::RunUpdate()
{
	if (vel.y == 0.f)
	{
		if (turnFlag)
		{
			vel.x = defSpeed;
		}
		else
		{
			vel.x = -defSpeed;
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
	vel.y = -0.5;		
	ProceedAnimFile();
}

void Enemy::DieUpdate()
{
	if (!dieFlag)
	{
		if (vel.y < defSpeed)
		{
			vel.y += fallAccel;
		}
		else
		{
			vel.x = 0;
			vel.y = defSpeed;
		}
	}
	
	
	ProceedAnimFile();
}

bool Enemy::IsRiseUnder(const Rect& bRect)
{
	auto isUnder = CollisionDetector::UnderCollCheck(bRect, GetRect());
	if (isUnder)
	{
		return true;
	}
	return false;
}

void Enemy::IsSide(const Rect & pRect, const Rect & wRect)
{
	auto isHit   = CollisionDetector::CollCheck(GetRect(), pRect);
	auto isWallSide = CollisionDetector::SideCollCheck(GetRect(), wRect);

	if (updater == &Enemy::BubbleUpdate &&
		updater != &Enemy::DieUpdate &&
		isHit)
	{
		if (isWallSide)
		{
			if (wRect.Right() > GetRect().Left())
			{
				/// ���S���āA�E�����ɔ��
				AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
				Die();
				turnFlag = true;
				vel.x = defSpeed;
				vel.y = -(defSpeed * 2);
			}
			else if (wRect.Left() < GetRect().Right())
			{
				/// ���S���āA�������ɔ��
				AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
				Die();
				turnFlag = false;
				vel.x = -defSpeed;
				vel.y = -(defSpeed * 2);
			}
			else {}
		}
	}
	else
	{
		if (updater != &Enemy::DieUpdate) { vel.x = 0.f; }
	}
}

bool Enemy::IsUnder(const Rect & pRect, const Input & p)
{
	auto isDir		 = (pRect.Right() < GetRect().center.x);						// true:�E�ɓ�������, false:���ɓ�������
	auto isHit		 = (CollisionDetector::CollCheck(GetRect(), pRect));
	auto underBubble = (CollisionDetector::UnderCollCheck(GetRect(), pRect));		// �A�̉����Ƃ̓����蔻��
	auto underPlayer = (CollisionDetector::UnderCollCheck(pRect, GetRect()));		// ��ڲ԰�̉����Ƃ̓����蔻��

	if (isHit && updater == &Enemy::BubbleUpdate)
	{
		/// �W�����v�{�^�����������ςȂ��̎�
		if (p.IsPressing(PAD_INPUT_1))
		{
			/// �v���C���[���n��ŖA��Ԃ̓G�ɓ�����ƁB�A�������
			if (underBubble || (GetRect().Top() < pRect.center.y + (size.y / 4)))
			{
				if (isDir)
				{
					AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
					Die();
					turnFlag = true;
					vel.x = defSpeed;
					vel.y = -(defSpeed * 2);
				}
				else
				{
					AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
					Die();
					turnFlag = false;
					vel.x = -defSpeed;
					vel.y = -(defSpeed * 2);
				}
				return false;
			}
			/// �{�^�������������Ă���ƁA�A�̏���Ԃ��Ƃ��ł���
			if (underPlayer)
			{
				return true;
			}
		}
		else
		{
			/// �W�����v�{�^���������Ă��Ȃ���Ԃ̎�
			if (underPlayer)
			{
				if (isDir)
				{
					/// ���S���āA�E�����ɔ��
					AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
					Die();
					turnFlag = true;
					vel.x = defSpeed;
					vel.y = -(defSpeed * 2);
				}
				else
				{
					/// ���S���āA�������ɔ��
					AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
					Die();
					turnFlag = false;
					vel.x = -defSpeed;
					vel.y = -(defSpeed * 2);
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