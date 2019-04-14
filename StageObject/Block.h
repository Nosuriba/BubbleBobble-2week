#pragma once
#include "StageObject.h"

class Input;

class Block :
	public StageObject
{
public:
	Block();
	~Block();
	void Update();
	void Draw();
	Rect GetRect();
private:
};

