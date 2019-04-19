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

	const bool& HitPlayer(const Rect& rcB, const Input& p);
	const bool& HitEnemy(const Rect& rcA, const Rect& rcB);
	const bool& HitObject(const Rect& rcA, const Rect& rcB);
	const bool& HitBubble(const Rect& rcA, const Rect& rcB);

	const bool& CheckShotState();
	const bool& CheckPopState();
	const bool& CheckDelete();

	void MoveContact(const Rect& rcB);

	const Vector2f& GetPos();
	Rect GetRect();
	Rect ShotGetRect();
private:
	void Shot();		// –A‚ğ“f‚¢‚½
	void Floating();	// –A‚ª•‚‚¢‚Ä‚¢‚é	
	void Pop();

	void ShotUpdate();
	void FloatingUpdate();
	void PopUpdate();

	void SideCheck(const Rect & player, const Rect& wall);
	const bool& GroundCheck(const Rect& rcB, const Input& p);
	const bool& CeilCheck();
	void DebugDraw();

	void (Bubble::*updater)();

	bool gameFlag;		// true:ƒQ[ƒ€’†, false:ƒQ[ƒ€’†‚Å‚È‚¢
	bool bubbleDir;		// true:¶•ûŒü, false:‰E•ûŒü
	bool deleteFlag;	// true:íœ, false:íœ‚µ‚È‚¢

	int bubbleImage;
	int invCnt;

	const int spitFrame;
	const float defSpeed;
	const float colSpeed;
};