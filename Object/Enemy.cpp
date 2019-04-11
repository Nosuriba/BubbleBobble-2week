#include "../Input.h"
#include "Enemy.h"

Enemy::Enemy()
{
	pos = vel = Vector2f(0, 0);
}

Enemy::Enemy(int groundLine)
{
	pos = vel = Vector2f(0, 0);
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

void Enemy::InitAnim(void)
{
}

void Enemy::AddAnim(std::string animName, const Vector2 & id, int frame, int interval)
{
	
}
