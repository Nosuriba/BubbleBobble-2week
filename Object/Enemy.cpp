#include "../Input.h"
#include "Enemy.h"

Enemy::Enemy()
{
	pos = vel = Vector2f(0, 0);
	divCnt = size = Vector2(0, 0);
	fileName = "";
}

Enemy::Enemy(int groundLine)
{
	pos = vel = Vector2f(0, 0);
	divCnt = size = Vector2(0, 0);
	fileName = "";
	this->groundLine = groundLine;
}

Enemy::~Enemy()
{
}

void Enemy::Update(const Input & p)
{
}

void Enemy::Draw()
{
}
