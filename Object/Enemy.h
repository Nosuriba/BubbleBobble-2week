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
	bool HitWall(const Rect& wall);							// 壁と当たったかの判定
	bool HitGround(const Rect& block);						// ｼﾞｬﾝﾌﾟ中、ﾌﾞﾛｯｸに乗ったかの判定
	bool UpperCheck(const Rect& player, const Rect& block);	// ﾌﾟﾚｲﾔｰが上にいるかの判定	

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
	int waitCnt;		/// 敵の待機時間用
};

