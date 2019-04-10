#pragma once
#include "CharactorObject.h"

class Input;

class Block :
	public CharactorObject
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

