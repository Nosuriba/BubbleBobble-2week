#pragma once
#include "CharactorObject.h"

class Bubble;
class Input;

class Bubble :
	public CharactorObject
{
public:
	Bubble();
	Bubble(const bool& bubbleDir);
	~Bubble();

	void Update();
	void Draw();
	void ChangePop();

	void HitAcross(const Rect & player, const Rect& wall);

	int HitPlayer(const Rect& rcB, const Input& p);
	const bool& HitEnemy(const Rect& rcA, const Rect& rcB);
	const bool& HitObject(const Rect& rcA, const Rect& rcB);
	const bool& HitBubble(const Rect& rcA, const Rect& rcB);

	const bool& CeilCheck();
	const bool& CheckFloating();
	const bool& CheckDelete();

	void MoveContact(const Rect& rcB);

	const Vector2f& GetPos();
	Rect GetRect();
	Rect ShotGetRect();
private:
	void Shot();		// 泡を吐いた時
	void Floating();	// 泡が浮いている時	
	void Pop();

	void ShotUpdate();
	void FloatingUpdate();
	void PopUpdate();

	void SideCheck(const Rect & player, const Rect& wall);
	int GroundCheck(const Rect& rcB, const Input& p);
	
	void DebugDraw();

	void (Bubble::*updater)();

	bool gameFlag;		// true:ゲーム中, false:ゲーム中でない
	bool bubbleDir;		// true:左方向, false:右方向
	bool deleteFlag;	// true:削除, false:削除しない

	int bubbleImage;
	int invCnt;

	const int spitFrame;
	const float defSpeed;
	const float colSpeed;
};