#pragma once
#include "CharactorObject.h"

class Input;

class Enemy :
	public CharactorObject
{
public:
	Enemy();
	Enemy(int groundLine);
	~Enemy();
	bool HitPlayer(const Rect& pRect, const Rect& wRect, const Input& p);
	bool HitBubble(const Rect& bblRect, const bool& bblCheck);

	// �ǂƂ̓����蔻��
	bool HitWall(const Rect& wRect);		

	// �W�����v���A�u���b�N�ɏ�������̔���
	bool HitGround(const Rect& pRect, const Rect& bRect);	

	// ����Ƀv���C���[�����邩�̔���
	bool UpperCheck(const Rect& pRect, const Rect& bRect);		

	// ���S���̐���p
	void DieControl(const Rect& objRect);						
	
	// �����m�F�p
	bool IsAlive();

	void Update();
	void Draw();
	Rect GetRect();
private:
	void Idle();
	void Run();
	void Jump();
	void Bubble();
	void Die();

	/// �֐��|�C���^�[�ŊǗ����s������
	void IdleUpdate();
	void RunUpdate();
	void JumpUpdate();
	void BubbleUpdate();
	void DieUpdate();

	// �㏸���ɏ����u���b�N�ł��邩�̊m�F�p
	bool IsRiseUnder(const Rect& bRect);
	void IsSide(const Rect& pRect, const Rect& wRect);
	bool IsUnder(const Rect& pRect, const Input& p);
	bool OnGround();

	void DebugDraw();

	void(Enemy::*updater)();

	int groundLine;		// ���̈ʒu
	int enemyImg;		// �G�̉摜ID�擾�p
	int waitCnt;		// �G�̑ҋ@���ԗp

	bool riseFlag;
};

