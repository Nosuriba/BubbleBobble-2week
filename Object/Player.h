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
	bool IsHitEnemy(const Rect& eRect, bool eAlive);	// 敵との当たり判定
	bool IsHitWall(const Rect& wRect);					// 壁との当たり判定
	bool isGround(const Rect& bRect);					// ジャンプ中、床に乗ったかの判定
	void IsStepBubble();								// 泡を踏んだかの判定
	bool IsShot();										// ショットを打てるかの判定

	void Update(const Input & p);
	void Draw();


	bool GetTurnFlag();				// プレイヤーの方向取得用
	const Vector2f& GetPos(); 
	Rect GetRect();					// 矩形取得用
private:
	void Idle();
	void Run();
	void Jump();
	void Shot();
	void Die();
	
	/// 関数ポインターで管理するもの
	void IdleUpdate(const Input & p);
	void RunUpdate(const Input & p);
	void JumpUpdate(const Input & p);
	void ShotUpdate(const Input & p);
	void DieUpdate(const Input & p);

	bool IsGround();	// 地面にいるかの判定取得用

	void DebugDraw();

	void (Player::*updater)(const Input & p);

	int playerImg;		// プレイヤーの画像ID取得用
	int invisiCnt;		// 無敵時間
	int startPos;		// スタート時の座標

	bool isHit;
	bool isShot;
};

