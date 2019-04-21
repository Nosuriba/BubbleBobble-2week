#include <DxLib.h>
#include "Enemy.h"
#include "../Game.h"
#include "../CollisionDetector.h"

Enemy::Enemy()
{
	Run();
	nowCutIdx = 0;
	ReadActionFile("Action/enemy.act");
	enemyImg = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	pos  = vel = Vector2f(0, 0);
	size = Vector2(0,0);
}

Enemy::Enemy(int groundLine)
{
	Run();
	nowCutIdx = 0;
	ReadActionFile("Action/enemy.act");
	enemyImg = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	this->groundLine = groundLine;
	pos  = vel = Vector2f(0, 0);
	jumpFlag = dieFlag = turnFlag = false;
	size = Vector2(0, 0);

	updater = &Enemy::RunUpdate;
}

Enemy::~Enemy()
{
	DxLib::DeleteGraph(enemyImg);
}

bool Enemy::HitBubble(const Rect & bRect)
{
	auto hitCheck = CollisionDetector::CollCheck(GetRect(), bRect);

	if (updater != &Enemy::BubbleUpdate && hitCheck)
	{
		Bubble();
		return true;
	}
	return false;
}

bool Enemy::HitWall(const Rect & wRect)
{
	auto hitCheck = CollisionDetector::SideCollCheck(GetRect(), wRect);
	if (hitCheck)
	{
		/// 壁に当たったら、方向転換するようにしている
		turnFlag = !turnFlag;
		pos.x	 = (turnFlag ? wRect.Right() : wRect.Left() - size.x);
	}

	return hitCheck;
}

bool Enemy::HitGround(const Rect & bRect)
{
	auto underCheck = CollisionDetector::UnderCollCheck(GetRect(), bRect);
	/// 落下中にブロックの上に乗った時の処理
	if (underCheck && vel.y >= 0.f && GetRect().Bottom() > (size.y + bRect.size.height))
	{
		this->vel.y = 0;
		this->groundLine = bRect.Top() + 1;		/// 床に少しめり込むようにしている。
	}
	else
	{
		this->groundLine = Game::GetInstance().GetScreenSize().y + (size.y * 2);
	}
	return underCheck;
}

bool Enemy::UpperCheck(const Rect& player, const Rect & block)
{
	return false;
}

Rect Enemy::GetRect()
{
	auto center = Vector2(pos.x + (size.x / 2), pos.y + (size.x / 2));
	auto rectSize = Size(size.x, size.y);

	return Rect(center, rectSize);
}

void Enemy::Idle()
{
	if (nowActionName != "run") { ChangeAction("run"); }
	vel = Vector2f(0, 0);
	updater = &Enemy::IdleUpdate;
}

void Enemy::Run()
{
	if (nowActionName != "run") { ChangeAction("run"); }
	updater = &Enemy::RunUpdate;
}

void Enemy::Jump()
{
	if (nowActionName != "run") { ChangeAction("run"); }
	updater = &Enemy::JumpUpdate;
}

void Enemy::Bubble()
{
	ChangeAction("bubble");
	vel.x = 0.f;
	updater = &Enemy::BubbleUpdate;
}

void Enemy::Die()
{
	ChangeAction("die");
	vel = Vector2f(0, 0);		// 速度は仮設定
	updater = &Enemy::DieUpdate;
}

void Enemy::IdleUpdate()
{

}

void Enemy::RunUpdate()
{
	if (turnFlag)
	{
		vel.x = 5.0f;
	}
	else
	{
		vel.x = -5.0f;
	}


	ProceedAnimFile();
}

void Enemy::JumpUpdate()
{
	
}

void Enemy::BubbleUpdate()
{
}

void Enemy::DieUpdate()
{
}

bool Enemy::OnGround()
{
	if (pos.y + size.y >= groundLine)
	{
		jumpFlag = false;
		return true;
	}
	return false;
}

void Enemy::Update()
{
	(this->*updater)();

	pos += vel;
}

void Enemy::Draw()
{
	CharactorObject::Draw(enemyImg);

#ifdef _DEBUG
	DebugDraw();
#endif 

}

void Enemy::DebugDraw()
{
	DrawString(0, 0, nowActionName.c_str(), 0xffffff);
	DrawBox(GetRect().Left(), GetRect().Top(), GetRect().Right(), GetRect().Bottom(), 0xff0000, false);
}