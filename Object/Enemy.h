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
	bool HitBubble(const Rect& bblRect);
	bool HitWall(const Rect& wRect);							// •Ç‚Æ“–‚½‚Á‚½‚©‚Ì”»’è
	bool HitGround(const Rect& bRect);						// ¼Ş¬İÌß’†AÌŞÛ¯¸‚Éæ‚Á‚½‚©‚Ì”»’è
	bool UpperCheck(const Rect& pRect, const Rect& bRect);	// ÌßÚ²Ô°‚ªã‚É‚¢‚é‚©‚Ì”»’è	

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

