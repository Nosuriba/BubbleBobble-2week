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
	void Anim();
	void Move(const Input & p);
	void Jump(const Input & p);
	void Fall();

	bool airFlag, groundFlag, dieFlag;
};

