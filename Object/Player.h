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
	bool HitWall(bool hitFlag, const Rect& rcB);			// �ǂƓ����������̔���
	bool HitGround(bool groundFlag, const Rect& rcB);		// �W�����v���A�u���b�N�ɏ�������̔���
	void Update(const Input & p);
	void Draw();
	Rect GetRect();									// ��`�擾�p
private:
	void Idle(const Input & p);
	void Run(const Input & p);
	void Jump(const Input & p);
	void Shot(const Input & p);
	bool OnGround();						// �n�ʂɂ��Ă��邩�̔���p

	void (Player::*updater)(const Input & p);

	void DebugDraw();

	int playerImg;

	bool hitFlag;
	bool jumpFlag;		// true:���ł���, false:���ł��Ȃ�
	bool dieFlag;		// true:�����Ă���, false:����ł���
};

