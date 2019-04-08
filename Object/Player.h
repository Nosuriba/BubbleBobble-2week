#pragma once
#include "Object.h"

class Input;

class Player :
	public Object
{
public:
	Player();
	Player(int groundLine);
	~Player();
	void InitAnim(void);
	void AddAnim(std::string animName, const Vector2 & id,
				 int frame, int interval);
	void Update(const Input & p);
	void Draw();
private:
	void ChangeAnim();
	void Anim();
	void Idle(const Input & p);
	void Run(const Input & p);
	void Jump(const Input & p);

	void (Player::*updater)(const Input & p);

	void DebugDraw();

	bool runFlag;		// true:�����Ă���, false:�����Ă��Ȃ�
	bool jumpFlag;		// true:���ł���, false:���ł��Ȃ�
	bool groundFlag;	// true:�n��, false:��
	bool dieFlag;		// true:�����Ă���, false:����ł���
    bool turnFlag;		// true:�E����, false:������
};

