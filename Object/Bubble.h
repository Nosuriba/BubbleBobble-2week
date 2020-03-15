#pragma once
#include "CharactorObject.h"

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

	// �A�������Ԃɂ���
	void ChangePop();

	void HitAcross(const Rect & pRect, const Rect& wRect);

	/// �A�ȊO�̃I�u�W�F�N�g�Ƃ̓����蔻��
	bool HitPlayer(const Rect& pRect, const Input& p);
	bool HitEnemy(const Rect& rcB);
	bool HitObject(const Rect& objRect);
	bool HitBubble(const Rect& bblRect);

	// ��̊m�F
	bool CeilCheck();

	// �폜���邩�̊m�F
	bool IsDelete();

	// �A���m�̐ڐG���s��ꂽ�Ƃ��̈ړ�
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

	// ���̊m�F
	void SideCheck(const Rect & pRect, const Rect& wRect);	

	// ���̊m�F
	bool UnderCheck(const Rect& pRect, const Input& p);		
	
	void DebugDraw();

	void (Bubble::*updater)();

	bool isGame;		// true:�Q�[����, false:�Q�[�����łȂ�
	bool bubbleDir;		// true:������, false:�E����
	bool isDelete;		// true:�폜, false:�폜���Ȃ�

	int bblImage;		// �A�̉摜ID�擾�p
	int invCnt;
};