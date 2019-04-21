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
	bool HitBubble(const Rect& bubble);
	bool HitWall(const Rect& wall);							// �ǂƓ����������̔���
	bool HitGround(const Rect& block);						// �ެ��ߒ��A��ۯ��ɏ�������̔���
	bool UpperCheck(const Rect& player, const Rect& block);	// ��ڲ԰����ɂ��邩�̔���	

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

	bool OnGround();

	void DebugDraw();

	void(Enemy::*updater)();

	int enemyImg;
	int waitCnt;		/// �G�̑ҋ@���ԗp
};

