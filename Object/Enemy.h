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
	bool HitWall(const Rect& wall);							// •Ç‚Æ“–‚½‚Á‚½‚©‚Ì”»’è
	bool HitGround(const Rect& block);						// ¼Ş¬İÌß’†AÌŞÛ¯¸‚Éæ‚Á‚½‚©‚Ì”»’è
	bool UpperCheck(const Rect& player, const Rect& block);	// ÌßÚ²Ô°‚ªã‚É‚¢‚é‚©‚Ì”»’è	

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
	int waitCnt;		/// “G‚Ì‘Ò‹@ŠÔ—p
};

