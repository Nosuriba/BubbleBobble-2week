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
	bool HitWall(const Rect& wall);			// �ǂƓ����������̔���
	bool HitGround(const Rect& block);		// �W�����v���A�u���b�N�ɏ�������̔���
	bool UpperCheck(const Rect& player, const Rect& block);		

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
};

