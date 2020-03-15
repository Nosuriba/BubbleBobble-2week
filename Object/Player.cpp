#include <DxLib.h>
#include <cmath>
#include "Player.h"
#include "../Input.h"
#include "../Game.h"
#include "../AudioMng.h"
#include "../CollisionDetector.h"

const int shotFrame	  = 5;
const int inviciFrame = 90;
	
Player::Player()
{
}

Player::Player(int groundLine)
{
	Idle();

	/// �摜�̏�����
	nowCutIdx	  = 0;
	ReadActionFile("Action/player.act");
	playerImg	  = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	this->groundLine = startPos = groundLine + 1;		/// ���̏�����
	pos		  = vel = Vector2f(0, 0);
	size	  = Vector2(0, 0);
	invisiCnt = 0;
	jumpFlag  = dieFlag = isHit = turnFlag = isShot = false;
	
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

bool Player::IsHitEnemy(const Rect & eRect, bool eAlive)
{
	auto IsEnemy = CollisionDetector::CollCheck(GetRect(), eRect);

	if (updater != &Player::DieUpdate && 
		IsEnemy && eAlive && invisiCnt < 0)
	{
		// �v���C���[�̎��S
		LpAudioMng.PlaySE(LpAudioMng.GetSound().die);
		Die();
		return true;
	}

	return false;
}

bool Player::IsHitWall(const Rect& wRect)
{
	this->isHit = CollisionDetector::SideCollCheck(GetRect(), wRect);
	if (isHit)
	{
		/// �ǂ̓��������ꏊ�ɂ���āA�ʒu�␳���s���Ă���
		vel.x = 0;
		pos.x = (turnFlag ? wRect.Right() : wRect.Left() - size.x);
	}

	return this->isHit;
}

bool Player::isGround(const Rect& bRect)
{
	auto underCheck = CollisionDetector::UnderCollCheck(GetRect(), bRect);
	/// �������Ƀu���b�N�̏�ɏ�������̏���
	if (underCheck && vel.y >= 0.f && pos.y > size.y * 2)
	{
 		this->vel.y		 = 0;
		this->groundLine = bRect.Top() + 1;			/// ���̍X�V
	}
	else
	{
		this->groundLine = Game::GetInstance().GetScreenSize().y + (size.y * 2);
	}
	return underCheck;
}

void Player::IsStepBubble()
{
	AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().bubble);
	AudioMng::GetInstance().ChangeVolume(80, AudioMng::GetInstance().GetSound().bubble);

	if (updater != &Player::DieUpdate)
	{
		vel.y = -12.0f;
	}	
}

bool Player::IsShot()
{
	if (isShot)
	{
		isShot = false;
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
	vel = Vector2f(0, 0);
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
		/// �V���b�g��ł�
		isShot = true;
		AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().shot);
		AudioMng::GetInstance().ChangeVolume(50, AudioMng::GetInstance().GetSound().shot);
		Shot();
	}

	/// �n�ʂɂ��Ă��邩�̔���
	if (IsGround())
	{
		vel	  = Vector2f(0, 0);
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
	if (!isHit)
	{
		if (p.IsPressing(PAD_INPUT_RIGHT))
		{
			/// �E�ړ�
			turnFlag = false;
			vel.x = defSpeed;
		}
		else if (p.IsPressing(PAD_INPUT_LEFT))
		{
			/// ���ړ�
			turnFlag = true;
			vel.x = -defSpeed;
		}
		else
		{
			vel.x = 0;
			Idle();
		}
	}

	if (p.IsTrigger(PAD_INPUT_2))
	{
		/// �V���b�g��ł�
		isShot = true;
		AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().shot);
		AudioMng::GetInstance().ChangeVolume(50, AudioMng::GetInstance().GetSound().shot);
		Shot();
	}

	/// �n�ʂɂ��Ă��邩�̔���
	if (IsGround())
	{
		vel.y = 0;
		if (p.IsTrigger(PAD_INPUT_1))
		{
			/// �W�����v���s��
			AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().jump);
			vel.y -= 14.0f;
			Jump();
		}
	}
	else
	{
		/// �󒆂ɂ����Ԃɂ���
		Jump();
	}
	ProceedAnimFile();
}

void Player::JumpUpdate(const Input & p)
{
	/// �n�ʂɂ��Ă��邩�̔���
	if (IsGround())
	{
		vel.y = 0;
		pos.y = groundLine - size.y;
		Idle();
		return;
	}
	else
	{
		///	��������
		vel.y = (vel.y < 0.5f ? vel.y += 0.7f : vel.y = 5.0);
	}

	if (p.IsTrigger(PAD_INPUT_2))
	{
		/// �V���b�g��ł�
		isShot = true;
		AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().shot);
		AudioMng::GetInstance().ChangeVolume(50,AudioMng::GetInstance().GetSound().shot);
		Shot();
	}

	if (!isHit)
	{
		if (p.IsPressing(PAD_INPUT_RIGHT))
		{
			/// �E�ړ�
			turnFlag = false;
			vel.x	 = defSpeed;
		}
		else if (p.IsPressing(PAD_INPUT_LEFT))
		{
			/// ���ړ�
			turnFlag = true;
			vel.x	 = -defSpeed;
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
	/// �n�ʂɂ��Ă��邩�̔���
	if (IsGround())
	{
		vel.y = 0;
		pos.y = groundLine - size.y;
	}
	else
	{
		///	��������
		vel.y = (vel.y < 0.5f ? vel.y += 0.7f : vel.y = 5.0);
	}

	/// �ǂɓ������Ă��Ȃ���
	if (!isHit)
	{
		if (p.IsPressing(PAD_INPUT_RIGHT))
		{
			turnFlag = false;
			vel.x = defSpeed;
		}
		else if (p.IsPressing(PAD_INPUT_LEFT))
		{
			turnFlag = true;
			vel.x = -defSpeed;
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
	/// �n�ʂɂ��Ă��邩�̔���
	if (IsGround())
	{
		vel.y = 0;
		pos.y = groundLine - size.y;
	}
	else
	{
		///	��������
		vel.y = (vel.y < 0.5f ? vel.y += 0.7f : vel.y = 5.0);
	}

	/// ���S���̃A�j���[�V�������I��������A�ҋ@��Ԃɂ���
	if (ProceedAnimFile())
	{
		Idle();
		invisiCnt = inviciFrame;
		vel = Vector2f(0, 0);
		pos = Vector2f(size.x * 2, startPos - size.y);
	}
}

bool Player::IsGround()
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
	if (pos.y > Game::GetInstance().GetScreenSize().y)
	{
		pos.y = -(size.y);
	}
	
	invisiCnt--;
	pos += vel;
}

void Player::Draw()
{
	if (invisiCnt > 0)
	{
		/// ���G���̃v���C���[�̕`��
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		CharactorObject::Draw(playerImg);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	else
	{
		CharactorObject::Draw(playerImg);
	}
	
#ifdef _DEBUG
	DebugDraw();
#endif 

}

void Player::DebugDraw()
{
	DrawBox(GetRect().Left(), GetRect().Top(), GetRect().Right(), GetRect().Bottom(), 0xff0000, false);
}