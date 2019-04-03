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
	void Update(const Input & p);
	void Draw();
private:
	void Anim();
	void Jump(const Input & p);
	void Fall();

	bool jumpFlag, groundFlag, dieFlag;
	int  jumpCnt;
	int  animCnt;
	int  invCnt;
};

