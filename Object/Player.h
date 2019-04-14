#pragma once
#include "../Object/CharactorObject.h"

class CollisionDetector;
class Input;

class Player :
	public CharactorObject
{
public:
	Player();
	Player(int groundLine);
	~Player();
	bool HitWall(bool hitFlag, const Rect& rcB);		// �ǂƓ����������̔���
	bool HitGround(bool groundFlag, const Rect& rcB);	// �W�����v���A�u���b�N�ɏ�������̔���
	bool CreateBubble();								// �V���b�g���łĂ邩�̔���p
	void Update(const Input & p);
	void Draw();
	Rect GetRect();									   // ��`�擾�p
private:
	void Idle();
	void Run();
	void Jump();
	void Shot();
	
	void IdleUpdate(const Input & p);
	void RunUpdate(const Input & p);
	void JumpUpdate(const Input & p);
	void ShotUpdate(const Input & p);

	bool OnGround();						// �n�ʂɂ��Ă��邩�̔���p

	void DebugDraw();

	void (Player::*updater)(const Input & p);

	int playerImg;
	int shotInvCnt;		// �V���b�g���łĂ�܂ł̊Ԋu

	bool hitFlag;
	bool shotFlag;
	bool jumpFlag;		// true:���ł���, false:���ł��Ȃ�
	bool dieFlag;		// true:�����Ă���, false:����ł���

	const int invCnt;
};

