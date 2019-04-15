#pragma once
#include "CharactorObject.h"

class Bubble :
	public CharactorObject
{
public:
	Bubble(const bool& bubbleDir);
	~Bubble();

	void Update();
	void Draw();
	Rect GetRect();
private:
	void Spit();		// �A��f������
	void Floating();	// �A�������Ă��鎞	

	void SpitUpdate();
	void FloatingUpdate();

	void (Bubble::*updater)();

	bool bubbleDir;		// true:������, false:�E����

	int bubbleImage;
	int invCnt;

	const int spitFrame;
};