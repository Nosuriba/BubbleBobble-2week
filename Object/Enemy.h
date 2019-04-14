#pragma once
#include "CharactorObject.h"

class Input;

class Enemy :
	public CharactorObject
{
public:
	Enemy();
	~Enemy();
	void Update(const Input & p);
	void Draw();
	Rect GetRect();
private:
	void Idle();
	void Run();
	void Jump();
	void Die();

	void IdleUpdate(const Input& p);
	void RunUpdate(const Input& p);
	void JumpUpdate(const Input& p);
	void DieUpdate(const Input& p);

	void(Enemy::*updater)(const Input& p);

	int enemyImage;
};

