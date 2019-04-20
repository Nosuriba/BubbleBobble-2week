#pragma once
#include "../Object/CharactorObject.h"

class CollisionDetector;
class Input;

class Player :
	public CharactorObject
{
public:
	Player();
	Player(int groundLine);
	~Player();
	bool HitWall(const Rect& wall);		// 壁と当たったかの判定
	bool HitGround(const Rect& ground);	// ジャンプ中、ブロックに乗ったかの判定
	void StepBubble();		// 泡を踏んだかの判定
	bool ShotCheck();									// ショットが打てるかの判定用

	void Update(const Input & p);
	void Draw();

	bool GetTurnFlag();
	const Vector2f& GetPos(); 
	Rect GetRect();									   // 矩形取得用
private:
	void Idle();
	void Run();
	void Jump();
	void Shot();
	void Die();
	
	void IdleUpdate(const Input & p);
	void RunUpdate(const Input & p);
	void JumpUpdate(const Input & p);
	void ShotUpdate(const Input & p);
	void DieUpdate(const Input & p);

	bool OnGround();						// 地面についているかの判定用

	void DebugDraw();

	void (Player::*updater)(const Input & p);

	int playerImg;
	int invCnt;			// ショットが打てるまでの間隔

	bool hitFlag;
	bool shotFlag;
	
	const int shotFrame;
};

