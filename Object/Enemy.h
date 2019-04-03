#pragma once
#include "Object.h"

class Input;

class Enemy :
	public Object
{
public:
	Enemy();
	Enemy(int groundLine);
	~Enemy();
	void Update(const Input & p);
	void Draw();
	void InitAnim(void);
	void AddAnim(std::string animName, const Vector2 & id,
				 int frame, int interval);
};

