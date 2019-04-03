#pragma once
#include "Object.h"

class Input;

class Block :
	public Object
{
public:
	Block();
	Block(int groundLine);
	~Block();
	void Update(const Input & p);
	void Draw();
};

