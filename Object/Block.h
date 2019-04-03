#pragma once
#include "Object.h"
class Block :
	public Object
{
public:
	Block();
	~Block();
	void Update();
	void Draw();
};

