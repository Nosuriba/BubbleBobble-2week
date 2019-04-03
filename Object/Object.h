#pragma once

#include <iostream>
#include "../Geometry.h"

class Input;

class Object
{
public:
	Object();
	~Object();
	virtual void Update(const Input & p);
	virtual void Draw();

	void Init(std::string fileName, const Vector2 & pos, const Vector2 & divCnt,const Vector2 & size);
protected:
	Vector2 pos;
	Vector2 divCnt;
	Vector2 size;
	// Vector2 divOffset;
	Vector2 vel;
	std::string fileName;
	

};

