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
	bool IsHitEnemy(const Rect& eRect, bool eAlive);	// �G�Ƃ̓����蔻��
	bool IsHitWall(const Rect& wRect);					// �ǂƂ̓����蔻��
	bool isGround(const Rect& bRect);					// �W�����v���A���ɏ�������̔���
	void IsStepBubble();								// �A�𓥂񂾂��̔���
	bool IsShot();										// �V���b�g��łĂ邩�̔���

	void Update(const Input & p);
	void Draw();


	bool GetTurnFlag();				// �v���C���[�̕����擾�p
	const Vector2f& GetPos(); 
	Rect GetRect();					// ��`�擾�p
private:
	void Idle();
	void Run();
	void Jump();
	void Shot();
	void Die();
	
	/// �֐��|�C���^�[�ŊǗ��������
	void IdleUpdate(const Input & p);
	void RunUpdate(const Input & p);
	void JumpUpdate(const Input & p);
	void ShotUpdate(const Input & p);
	void DieUpdate(const Input & p);

	bool IsGround();	// �n�ʂɂ��邩�̔���擾�p

	void DebugDraw();

	void (Player::*updater)(const Input & p);

	int playerImg;		// �v���C���[�̉摜ID�擾�p
	int invisiCnt;		// ���G����
	int startPos;		// �X�^�[�g���̍��W

	bool isHit;
	bool isShot;
};

