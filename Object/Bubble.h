#pragma once
#include "CharactorObject.h"

class Bubble :
	public CharactorObject
{
	Bubble();
	~Bubble();

	void Update();
	void Draw();
	Rect GetRect();
private:
	void Spit();		// �A��f������
	void Floating();	// �A�������Ă��鎞

	void SpitUpdate();
	void FloatingUpdate();

	void(Bubble::*updater)();
	int bubbleImage;
};