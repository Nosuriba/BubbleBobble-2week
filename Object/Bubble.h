#pragma once
#include "CharactorObject.h"

class Bubble;

class Bubble :
	public CharactorObject
{
public:
	Bubble(const bool& bubbleDir);
	~Bubble();

	void Update();
	void Draw();
	void ChangePop();
	const bool& HitPlayer(const bool& hitFlag);
	const bool& HitEnemy(const bool& hitFlag);
	const bool& HitObject(const bool& hitFlag);
	const bool& HitBubble(const bool& hitFlag, const bool& accelFlag);
	const bool& CheckShotState();
	const bool& CheckPopState();
	const bool& CheckDelete();
	bool CheckDebugF();			/// 浮いてる状態かの確認用(後で絶対消す)
	Rect GetRect();
	Rect ShotGetRect();
private:
	void Shot();		// 泡を吐いた時
	void Floating();	// 泡が浮いている時	
	void Pop();

	void ShotUpdate();
	void FloatingUpdate();
	void PopUpdate();

	void DebugDraw();

	void (Bubble::*updater)();

	bool bubbleDir;		// true:左方向, false:右方向
	bool deleteFlag;	// true:削除, false:削除しない

	int bubbleImage;
	int invCnt;

	const int spitFrame;
};