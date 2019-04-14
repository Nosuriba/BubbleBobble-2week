#include <DxLib.h>
#include "../Input.h"
#include "Enemy.h"

Enemy::Enemy()
{
	Idle();
	nowCutIdx = 0;
	ReadActionFile("Action/enemy.act");
	enemyImage = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	pos = vel = Vector2f(0, 0);
	size = Vector2(0,0);
}

Enemy::~Enemy()
{
}

void Enemy::Idle()
{
}

void Enemy::Run()
{
}

void Enemy::Jump()
{
}

void Enemy::Die()
{
}

void Enemy::IdleUpdate(const Input & p)
{
}

void Enemy::RunUpdate(const Input & p)
{
}

void Enemy::JumpUpdate(const Input & p)
{
}

void Enemy::DieUpdate(const Input & p)
{
}

void Enemy::Update(const Input & p)
{
	(this->*updater)(p);
}

void Enemy::Draw()
{
}
