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
	bool HitWall(const Rect& wall);		// �ǂƓ����������̔���
	bool HitGround(const Rect& ground);	// �W�����v���A�u���b�N�ɏ�������̔���
	void StepBubble();		// �A�𓥂񂾂��̔���
	bool ShotCheck();									// �V���b�g���łĂ邩�̔���p

	void Update(const Input & p);
	void Draw();

	bool GetTurnFlag();
	const Vector2f& GetPos(); 
	Rect GetRect();									   // ��`�擾�p
private:
	void Idle();
	void Run();
	void Jump();
	void Shot();
	void Die();
	
	void IdleUpdate(const Input & p);
	void RunUpdate(const Input & p);
	void JumpUpdate(const Input & p);
	void ShotUpdate(const Input & p);
	void DieUpdate(const Input & p);

	bool OnGround();						// �n�ʂɂ��Ă��邩�̔���p

	void DebugDraw();

	void (Player::*updater)(const Input & p);

	int playerImg;
	int invCnt;			// �V���b�g���łĂ�܂ł̊Ԋu

	bool hitFlag;
	bool shotFlag;
	
	const int shotFrame;
};

