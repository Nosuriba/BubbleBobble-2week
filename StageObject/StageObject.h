#pragma once
#include <iostream>
#include "../Geometry.h"

class StageObject
{
public:
	StageObject();
	~StageObject();
	virtual void Update();
	virtual void Draw();
	virtual Rect GetRect();
	// ̧�ٖ�, ���W, �摜�̍ő啪����, �摜�̕`��ʒu, �摜�T�C�Y
	void Init(std::string fileName, const Vector2f & pos, const Vector2 & divCnt, const Vector2 & divOffset, const Vector2 & size);
protected:
	Vector2f GetPos() const;

	Vector2f pos;
	Vector2 size;
	Vector2 divCnt;
	Vector2 divOffset;

	std::string fileName;

	int objImage;
	int chipCnt;			// �`�悷��摜�̈ʒu
};

