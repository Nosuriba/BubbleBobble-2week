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
	void Spit();		// –A‚ð“f‚¢‚½Žž
	void Floating();	// –A‚ª•‚‚¢‚Ä‚¢‚éŽž

	void SpitUpdate();
	void FloatingUpdate();

	void(Bubble::*updater)();
	int bubbleImage;
};