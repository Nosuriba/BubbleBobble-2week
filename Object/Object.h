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

	// ファイル名, 座標, 画像分割数, 画像位置, 画像の最大分割数, 画像サイズ
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

