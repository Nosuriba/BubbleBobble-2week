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
	void DebugDraw();
	bool HitPlayer(const bool hitFlag);
	bool HitEnemy(const bool hitFlag);
	bool HitObject(const bool hitFlag);
	bool HitBubble(const bool hitFlag);
	bool CheckShotState();
	bool CheckDelete();
	Rect GetRect();
	Rect ShotGetRect();
	
private:
	void Shot();		// �A��f������
	void Floating();	// �A�������Ă��鎞	
	void Pop();


	void ShotUpdate();
	void FloatingUpdate();
	void PopUpdate();

	void (Bubble::*updater)();

	bool bubbleDir;		// true:������, false:�E����
	bool deleteFlag;	// true:�폜, false:�폜���Ȃ�

	int bubbleImage;
	int invCnt;

	const int spitFrame;
};