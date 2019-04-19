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
	
	// ｱｸｼｮﾝ名, 座標, 画像サイズ
	void Init(std::string actionName, const Vector2f & pos, const Vector2 & size);

protected:
	void ChangeAction(const char * name);		// ｱｸｼｮﾝ切り替え
	bool ProceedAnimFile();						// ｱﾆﾒｰｼｮﾝﾌﾚｰﾑを1進める
	void ReadActionFile(const char* actionPath);// ﾌｧｲﾙの読み込み
	Vector2f GetPos() const;

	ObjectData actionData;

	Vector2f pos;
	Vector2f vel;
	Vector2 size;

	std::string nowActionName;

	int groundLine;
	int nowCutIdx;
	unsigned int frame;

	bool jumpFlag;		// true:ｼﾞｬﾝﾌﾟ中, false:ｼﾞｬﾝﾌﾟしていない
	bool turnFlag;		// true:左向き, false:右向き
	bool dieFlag;		// true:死亡, false:生存
};