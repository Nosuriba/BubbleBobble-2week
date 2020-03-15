#pragma once

#include <vector>
#include <map>
#include "../Geometry.h"

enum class RectType
{
	ANCHOR,		// アンカー
	ATTACK,		// 攻撃矩形
	DAMAGE,		// やられ矩形
	MAX
};

struct ActRect
{
	RectType rt;	// 矩形の状態
	Rect     rc;	// 矩形の幅と高さ
};

struct CutInfo
{
	Rect rect;			// 切り取り矩形
	Vector2 center;		// 中心点
	int duration;		// 表示時間
	std::vector<ActRect> actRects;	// アクション矩形
};

struct ObjectInfo
{
	bool isLoop;				// アニメーションのループ判定	
	std::vector<CutInfo> cuts;	// 切り取った情報を保存する配列		
};

struct ObjectData
{
	std::string imgFilePath;						// 画像のファイルパス
	std::map<std::string, ObjectInfo> actionInfo;	// アクション情報
};

class Object
{
public:
	Object()
	{

	}
	~Object()
	{

	}
};

