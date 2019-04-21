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
	bool HitBubble(const Rect& bblRect);
	bool HitWall(const Rect& wRect);						// �ǂƓ����������̔���
	bool HitGround(const Rect& bRect);						// �ެ��ߒ��A��ۯ��ɏ�������̔���
	bool UpperCheck(const Rect& pRect, const Rect& bRect);	// ��ڲ԰����ɂ��邩�̔���	
	void DieControl(const Rect& objRect);	// ���S���̐���p
	
	const bool& GetDieFlag();
	void Update();
	void Draw();
	Rect GetRect();
private:
	void Idle();
	void Run();
	void Jump();
	void Bubble();
	void Die();

	void IdleUpdate();
	void RunUpdate();
	void JumpUpdate();
	void BubbleUpdate();
	void DieUpdate();

	void SideCheck(const Rect& pRect, const Rect& wRect);
	bool UnderCheck(const Rect& pRect, const Input& p);
	bool OnGround();

	void DebugDraw();

	void(Enemy::*updater)();

	int enemyImg;
	int waitCnt;		/// �G�̑ҋ@���ԗp

	const float fallAccel;
};

