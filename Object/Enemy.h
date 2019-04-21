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
	bool HitWall(const Rect& wRect);						// 壁と当たったかの判定
	bool HitGround(const Rect& bRect);						// ｼﾞｬﾝﾌﾟ中、ﾌﾞﾛｯｸに乗ったかの判定
	bool UpperCheck(const Rect& pRect, const Rect& bRect);	// ﾌﾟﾚｲﾔｰが上にいるかの判定	
	void DieControl(const Rect& objRect);	// 死亡時の制御用
	
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

	bool RiseCheck(const Rect& bRect);
	void SideCheck(const Rect& pRect, const Rect& wRect);
	bool UnderCheck(const Rect& pRect, const Input& p);
	bool OnGround();

	void DebugDraw();

	void(Enemy::*updater)();

	int groundLine;
	int enemyImg;
	int waitCnt;		/// 敵の待機時間用

	bool riseFlag;

	const float fallAccel;
};

