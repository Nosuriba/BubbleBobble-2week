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

	bool runFlag;		// true:�����Ă���, false:�����Ă��Ȃ�
	bool jumpFlag;		// true:���ł���, false:���ł��Ȃ�
	bool groundFlag;	// true:�n��, false:��
	bool dieFlag;		// true:�����Ă���, false:����ł���

};

