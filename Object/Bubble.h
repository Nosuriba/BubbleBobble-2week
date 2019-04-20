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

	bool HitPlayer(const Rect& rcB, const Input& p);
	bool HitEnemy(const Rect& rcA, const Rect& rcB);
	bool HitObject(const Rect& rcA, const Rect& rcB);
	bool HitBubble(const Rect& rcA, const Rect& rcB);

	bool CeilCheck();
	bool CheckFloating();
	bool CheckDelete();

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

	void SideCheck(const Rect & player, const Rect& wall);
	bool GroundCheck(const Rect& rcB, const Input& p);
	
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