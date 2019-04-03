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
	void Move(const Input & p);

	bool jumpFlag, groundFlag, dieFlag;
};

