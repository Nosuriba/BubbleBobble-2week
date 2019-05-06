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

	void HitAcross(const Rect & pRect, const Rect& wRect);

	bool HitPlayer(const Rect& pRect, const Input& p);
	bool HitEnemy(const Rect& rcB);
	bool HitObject(const Rect& objRect);
	bool HitBubble(const Rect& bblRect);

	bool CeilCheck();
	bool CheckDelete();

	void MoveContact(const Rect& bblRect);

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

	void SideCheck(const Rect & pRect, const Rect& wRect);	
	bool UnderCheck(const Rect& pRect, const Input& p);		
	
	void DebugDraw();

	void (Bubble::*updater)();

	bool gameFlag;		// true:�Q�[����, false:�Q�[�����łȂ�
	bool bubbleDir;		// true:������, false:�E����
	bool deleteFlag;	// true:�폜, false:�폜���Ȃ�

	int bubbleImage;
	int invCnt;
};