#pragma once

#include <iostream>
#include "../Geometry.h"

class Input;

class Object
{
public:
	Object();
	Object(int groundLine);
	~Object();
	virtual void Update(const Input & p);
	virtual void Draw();

	// �t�@�C����, ���W, �摜������, �摜�ʒu, �摜�̍ő啪����, �摜�T�C�Y
	void Init(std::string fileName, const Vector2f & pos,
			const Vector2 & divCnt, const Vector2 & divOffset,
			const Vector2 & size);
protected:
	Vector2f pos;
	Vector2f vel;
	Vector2 divCnt;
	Vector2 divOffset;
	Vector2 size;
	std::string fileName;
	
	int chipCnt;
	int groundLine;
};

