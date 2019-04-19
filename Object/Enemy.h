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
	const bool& HitWall(const Rect& rcB);		// 壁と当たったかの判定
	const bool& HitGround(const Rect& rcB);	// ジャンプ中、ブロックに乗ったかの判定

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

	const bool& OnGround();

	void DebugDraw();

	void(Enemy::*updater)();

	int enemyImg;
};

