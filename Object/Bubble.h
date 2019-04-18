#pragma once
#include "CharactorObject.h"

class Bubble;

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
	const bool& HitPlayer(const bool& hitFlag);
	const bool& HitEnemy(const bool& hitFlag);
	const bool& HitObject(const bool& hitFlag);
	const bool& HitBubble(const bool& hitFlag);
	const bool& CheckShotState();
	const bool& CheckPopState();
	const bool& CheckDelete();
	void MoveContact(const Rect& rcB);
	const Vector2f& GetPos();
	Rect GetRect();
	Rect ShotGetRect();
private:
	void Shot();		// �A��f������
	void Floating();	// �A�������Ă��鎞	
	void Pop();

	void ShotUpdate();
	void FloatingUpdate();
	void PopUpdate();

	const bool& CeilCheck();
	void DebugDraw();

	void (Bubble::*updater)();

	bool gameFlag;		// true:�Q�[����, false:�Q�[�����łȂ�
	bool bubbleDir;		// true:������, false:�E����
	bool deleteFlag;	// true:�폜, false:�폜���Ȃ�

	int bubbleImage;
	int invCnt;

	const int spitFrame;
	const float defSpeed;
	const float colSpeed;
};