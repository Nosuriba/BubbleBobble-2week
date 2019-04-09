#pragma once
#include "Object.h"

class HitCheck;
class Input;

class Player :
	public Object
{
public:
	Player();
	Player(int groundLine);
	~Player();
	void CheckHit(bool hitFlag);
	void InitAnim(void);
	void AddAnim(std::string animName, const Vector2 & id,
				 int frame, int interval);
	void Update(const Input & p);
	void Draw();
	Rect GetRect();
private:
	void Anim();
	void Idle(const Input & p);
	void Run(const Input & p);
	void Jump(const Input & p);

	void (Player::*updater)(const Input & p);

	void DebugDraw();

	bool runFlag;		// true:‘–‚Á‚Ä‚¢‚é, false:‘–‚Á‚Ä‚¢‚È‚¢
	bool jumpFlag;		// true:”ò‚ñ‚Å‚¢‚é, false:”ò‚ñ‚Å‚¢‚È‚¢
	bool groundFlag;	// true:’nã, false:‹ó’†
	bool dieFlag;		// true:¶‚«‚Ä‚¢‚é, false:€‚ñ‚Å‚¢‚é
    bool turnFlag;		// true:‰EŒü‚«, false:¶Œü‚«
};

