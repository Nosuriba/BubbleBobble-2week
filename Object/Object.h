#pragma once

#include <iostream>
#include <map>
#include "../Geometry.h"

enum class ANIM
{
	START,
	FRAME,
	INTERVAL,
	MAX
};

class Input;

class Object
{
public:
	Object();
	Object(int groundLine);
	~Object();
	virtual void Update(const Input & p);
	virtual void Draw();
	virtual void CheckHit(bool hitFlag);
	virtual Rect GetRect();
	void SetAnim(std::string animName);


	// �t�@�C����, ���W, �摜�̍ő啪����, �摜�̕`��ʒu, �摜�T�C�Y
	void Init(std::string fileName, const Vector2f & pos,
			const Vector2 & divCnt, const Vector2 & divOffset, const Vector2 & size);

	// �t�@�C����, �A�j���[�V������, ���W, �摜�̍ő啪����, �摜�T�C�Y
	void Init(std::string fileName, std::string animName,
			  const Vector2f & pos, const Vector2 & divCnt, const Vector2 & size);
protected:

	virtual void InitAnim(void) = 0;
	virtual void AddAnim(std::string animName, const Vector2 & id,
						 int frame, int interval) = 0;

	void (Object::*updater)(const Input & p); 
	Vector2f pos;
	Vector2f vel;
	Vector2 divCnt;
	Vector2 size;

	std::string fileName;
	std::string animName;
	std::map<std::string, int[static_cast<int>(ANIM::MAX)]> animType;

	int invCnt;
	int chipCnt;
	int groundLine;

	bool hitFlag;
};

