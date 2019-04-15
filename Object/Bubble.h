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
	void Spit();		// –A‚ğ“f‚¢‚½
	void Floating();	// –A‚ª•‚‚¢‚Ä‚¢‚é	

	void SpitUpdate();
	void FloatingUpdate();

	void (Bubble::*updater)();

	bool bubbleDir;		// true:¶•ûŒü, false:‰E•ûŒü

	int bubbleImage;
	int invCnt;

	const int spitFrame;
};