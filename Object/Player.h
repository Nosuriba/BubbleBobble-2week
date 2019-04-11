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
	bool HitWall(bool hitFlag, Rect rcB);						// �ǂƓ����������̔���
	void HitGround(bool groundFlag, Rect rcB);		// �W�����v���A�u���b�N�ɏ�������̔���
	void Update(const Input & p);
	void Draw();
	Rect GetRect();									// ��`�擾�p
private:
	void Idle(const Input & p);
	void Run(const Input & p);
	void Jump(const Input & p);
	void Shot(const Input & p);
	void OnGround();

	void (Player::*updater)(const Input & p);

	void DebugDraw();

	std::string actionName;

	int playerImg;

	bool hitFlag;
	bool runFlag;		// true:�����Ă���, false:�����Ă��Ȃ�
	bool jumpFlag;		// true:���ł���, false:���ł��Ȃ�
	bool groundFlag;	// true:�n��, false:��
	bool dieFlag;		// true:�����Ă���, false:����ł���

	int debugLine;	   // �ŏ��ɐݒ肵�����̈ʒu��ۑ����Ă���(debug�p)

};

