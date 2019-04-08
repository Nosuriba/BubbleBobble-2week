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
	void SetAnim(std::string animName);

	// ファイル名, アニメーション名, 座標, 画像の最大分割数, 画像サイズ
	void Init(std::string fileName, std::string animName,
			  const Vector2f & pos, const Vector2 & divCnt, const Vector2 & size);
protected:

	virtual void InitAnim(void) = 0;
	virtual void AddAnim(std::string animName, const Vector2 & id,
						 int frame, int interval) = 0;
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
};

