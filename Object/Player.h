#pragma once
#include "../Object/CharactorObject.h"

class HitCheck;
class Input;

class Player :
	public CharactorObject
{
public:
	Player();
	Player(int groundLine);
	~Player();
	void CheckHit(bool hitFlag);
	void Update(const Input & p);
	void Draw();
	Rect GetRect();
private:
	void Idle(const Input & p);
	void Run(const Input & p);
	void Jump(const Input & p);

	void (Player::*updater)(const Input & p);

	void DebugDraw();

	std::string actionName;

	int playerImg;

	bool runFlag;		// true:‘–‚Á‚Ä‚¢‚é, false:‘–‚Á‚Ä‚¢‚È‚¢
	bool jumpFlag;		// true:”ò‚ñ‚Å‚¢‚é, false:”ò‚ñ‚Å‚¢‚È‚¢
	bool groundFlag;	// true:’nã, false:‹ó’†
	bool dieFlag;		// true:¶‚«‚Ä‚¢‚é, false:€‚ñ‚Å‚¢‚é

};

