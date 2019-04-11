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
	bool HitWall(bool hitFlag, Rect rcB);						// 壁と当たったかの判定
	void HitGround(bool groundFlag, Rect rcB);		// ジャンプ中、ブロックに乗ったかの判定
	void Update(const Input & p);
	void Draw();
	Rect GetRect();									// 矩形取得用
private:
	void Idle(const Input & p);
	void Run(const Input & p);
	void Jump(const Input & p);
	void Shot(const Input & p);
	void OnGround();

	void (Player::*updater)(const Input & p);

	void DebugDraw();

	std::string actionName;

	int playerImg;

	bool hitFlag;
	bool runFlag;		// true:走っている, false:走っていない
	bool jumpFlag;		// true:飛んでいる, false:飛んでいない
	bool groundFlag;	// true:地上, false:空中
	bool dieFlag;		// true:生きている, false:死んでいる

	int debugLine;	   // 最初に設定した床の位置を保存している(debug用)

};

