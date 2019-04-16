#pragma once
#include "CharactorObject.h"

class Bubble;
using shared_Bubble = std::shared_ptr<Bubble>;

class Bubble :
	public CharactorObject
{
public:
	Bubble(const bool& bubbleDir);
	~Bubble();

	void Update();
	void Draw();
	bool HitPlayer(const bool hitFlag);
	bool HitEnemy(const bool hitFlag);
	bool HitObject(const bool hitFlag);
	bool HitBubble(const bool hitFlag);
	bool CheckShotState();
	Rect GetRect();
	Rect ShotGetRect();
	shared_Bubble DeleteBubble(const shared_Bubble & itr);
private:
	void Shot();		// –A‚ğ“f‚¢‚½
	void Floating();	// –A‚ª•‚‚¢‚Ä‚¢‚é	
	void Pop();


	void ShotUpdate();
	void FloatingUpdate();
	void PopUpdate();

	void (Bubble::*updater)();

	bool bubbleDir;		// true:¶•ûŒü, false:‰E•ûŒü
	bool deleteFlag;	// true:íœ, false:íœ‚µ‚È‚¢

	int bubbleImage;
	int invCnt;

	const int spitFrame;
};