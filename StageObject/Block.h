#pragma once
#include "StageObject.h"

class Input;

class Block :
	public StageObject
{
public:
	Block();
	~Block();
	void Update(const Input & p);
	void Draw();
	Rect GetRect();
private:
};

