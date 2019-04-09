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
	Rect GetRect();
	void InitAnim(void);
	void AddAnim(std::string animName, const Vector2 & id,
				 int frame, int interval);
};

