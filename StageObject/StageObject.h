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
	// ﾌｧｲﾙ名, 座標, 画像の最大分割数, 画像の描画位置, 画像サイズ
	void Init(std::string fileName, const Vector2f & pos, const Vector2 & divCnt, const Vector2 & divOffset, const Vector2 & size);
protected:
	Vector2f GetPos() const;

	Vector2f pos;
	Vector2 size;
	Vector2 divCnt;
	Vector2 divOffset;

	std::string fileName;

	int objImage;
	int chipCnt;			// 描画する画像の位置
};

