#pragma once
#include "../Object/Object.h"
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

class CharactorObject : Object
{
public:
	CharactorObject();
	CharactorObject(int groundLine);
	~CharactorObject();
	void Draw(int img);
	virtual Rect GetRect();
	
	// ｱｸｼｮﾝ名, 座標, 画像サイズ
	void Init(std::string actionName, const Vector2f & pos, const Vector2 & size);
	// ﾌｧｲﾙ名, 座標, 画像の最大分割数, 画像の描画位置, 画像サイズ
	void Init(std::string fileName, const Vector2f & pos, const Vector2 & divCnt, const Vector2 & divOffset, const Vector2 & size);

protected:
	void ChangeAction(const char * name);		// アクション切り替え
	bool ProceedAnimFile();						// ｱﾆﾒｰｼｮﾝﾌﾚｰﾑを1進める
	void ReadActionFile(const char* actionPath);// ﾌｧｲﾙの読み込み

	Vector2f GetPos() const;

	ObjectData actionData;

	Vector2f pos;
	Vector2f vel;
	Vector2 size;
	Vector2 divCnt;
	Vector2 divOffset;

	std::string fileName;
	std::string nowActionName;
	
	int chipCnt;
	int groundLine;
	int nowCutIdx;
	unsigned int frame;

	bool hitFlag;
	bool turnFlag;		// true:右向き, false:左向き
};