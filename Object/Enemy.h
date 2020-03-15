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

	// 壁との当たり判定
	bool HitWall(const Rect& wRect);		

	// ジャンプ中、ブロックに乗ったかの判定
	bool HitGround(const Rect& pRect, const Rect& bRect);	

	// 頭上にプレイヤーがいるかの判定
	bool UpperCheck(const Rect& pRect, const Rect& bRect);		

	// 死亡時の制御用
	void DieControl(const Rect& objRect);						
	
	// 生存確認用
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

	/// 関数ポインターで管理を行うもの
	void IdleUpdate();
	void RunUpdate();
	void JumpUpdate();
	void BubbleUpdate();
	void DieUpdate();

	// 上昇中に床がブロックであるかの確認用
	bool IsRiseUnder(const Rect& bRect);
	void IsSide(const Rect& pRect, const Rect& wRect);
	bool IsUnder(const Rect& pRect, const Input& p);
	bool OnGround();

	void DebugDraw();

	void(Enemy::*updater)();

	int groundLine;		// 床の位置
	int enemyImg;		// 敵の画像ID取得用
	int waitCnt;		// 敵の待機時間用

	bool riseFlag;
};

