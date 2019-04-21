#include <DxLib.h>
#include "Enemy.h"
#include "../Game.h"
#include "../Input.h"
#include "../CollisionDetector.h"
#include "../AudioMng.h"

Enemy::Enemy() : fallAccel(0.3f)
{
	Run();
	nowCutIdx = 0;
	ReadActionFile("Action/enemy.act");
	enemyImg = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	pos  = vel = Vector2f(0, 0);
	size = Vector2(0,0);
}

Enemy::Enemy(int groundLine) : fallAccel(0.3f)
{
	Run();
	nowCutIdx = 0;
	ReadActionFile("Action/enemy.act");
	enemyImg = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	this->groundLine = groundLine;
	pos  = vel = Vector2f(0, 0);
	jumpFlag = dieFlag = turnFlag = riseFlag = false;
	size = Vector2(0, 0);


	updater = &Enemy::RunUpdate;
}

Enemy::~Enemy()
{
	DxLib::DeleteGraph(enemyImg);
}

Rect Enemy::GetRect()
{
	auto center = Vector2(pos.x + (size.x / 2), pos.y + (size.x / 2));
	auto rectSize = Size(size.x, size.y);

	return Rect(center, rectSize);
}

const bool & Enemy::GetDieFlag()
{
	return dieFlag;
}

bool Enemy::HitPlayer(const Rect & pRect, const Rect& wRect, const Input& p)
{
	SideCheck(pRect, wRect);
	return UnderCheck(pRect, p);
}

bool Enemy::HitBubble(const Rect & bRect)
{
	auto hitCheck = CollisionDetector::CollCheck(GetRect(), bRect);

	if (updater != &Enemy::BubbleUpdate && 
		updater != &Enemy::DieUpdate	&& hitCheck)
	{
		Bubble();
		return true;
	}
	return false;
}

bool Enemy::HitWall(const Rect & wRect)
{
	auto hitCheck = CollisionDetector::CollCheck(GetRect(), wRect);
	if (hitCheck && updater != &Enemy::DieUpdate)
	{
		/// 壁に当たったら、方向転換するようにしている
		turnFlag = !turnFlag;
		vel.x = (vel.x == charSpeed ? -charSpeed : charSpeed);
		return true;
	}

	return false;
}

bool Enemy::HitGround(const Rect & bRect)
{
	auto underCheck = CollisionDetector::UnderCollCheck(GetRect(), bRect);
	
	if (updater != &Enemy::DieUpdate)
	{
		/// 落下中にブロックの上に乗った時の処理
		if (underCheck && vel.y >= 0.f && GetRect().Bottom() > (size.y + bRect.size.height))
		{
			this->vel.y = 0;
			this->groundLine = bRect.Top() + 1;		/// 床に少しめり込むようにしている。
			return true;
		}
		this->groundLine = Game::GetInstance().GetScreenSize().y + (size.y * 2);
	}
	return false;
}

bool Enemy::UpperCheck(const Rect& pRect, const Rect & bRect)
{
	if (updater == &Enemy::RunUpdate)
	{
		if (pos.y > pRect.Bottom() &&
		   (int)pos.x == (int)bRect.center.x)
		{
			groundLine = 0;
			riseFlag = true;
			Idle();
		}
		return false;
	}

	if (updater == &Enemy::IdleUpdate && waitCnt < 0)
	{
		/// ここで敵が上昇するかの判定を取れるようにする
		if (riseFlag)
		{
			vel.y = -charSpeed;
			RiseCheck(bRect);
			return true;
		}
		else
		{
			/// 敵がﾌﾞﾛｯｸの上になった時、状態を変える(ようになってるから上昇判定どうにかしろ)
			if ((int)GetRect().Bottom() <= groundLine)
			{
				vel.y = 0;
				Run();
			}
		}
		
	}
	return false;
}

void Enemy::DieControl(const Rect& objRect)
{
	auto selHitCheck = [=](const Rect& objRect)
	{
		if (objRect.size.height == size.y)
		{
			/// 壁との当たり判定
			return CollisionDetector::CollCheck(GetRect(), objRect);
		}
		else
		{
			/// ﾌﾞﾛｯｸとの当たり判定
			return CollisionDetector::UnderCollCheck(GetRect(), objRect);
		}
	};
	/// ｵﾌﾞｼﾞｪｸﾄのｻｲｽﾞ確認用
	auto sizeCheck = (size.y == objRect.size.height ? true : false);

	if (updater == &Enemy::DieUpdate)
	{
		if (selHitCheck(objRect) && !sizeCheck && vel.y == charSpeed)
		{
			/// 床に当たった時、死亡状態にする(後で、敵が消えるようにする)
			vel		= Vector2f(0, 0);
			dieFlag = true;
			return;
		}

		if (selHitCheck(objRect) && sizeCheck)
		{
			/// 壁に当たった時に敵が跳ね返るようにしている
			pos.x = (vel.x == charSpeed ? objRect.Left() - size.x : objRect.Right());	
			vel.x = (vel.x == charSpeed ? -charSpeed : charSpeed);
		}
	}
}

void Enemy::Idle()
{
	if (nowActionName != "run") { ChangeAction("run"); }
	waitCnt = 60;
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
	updater = &Enemy::DieUpdate;
}

void Enemy::IdleUpdate()
{
	waitCnt--;
	if (waitCnt < 0)
	{
		//Jump();
		return;
	}
	auto changeTurn = ((waitCnt / 20) % 2 ? true : false);
	turnFlag = changeTurn;
	
	ProceedAnimFile();
}

void Enemy::RunUpdate()
{
	if (turnFlag)
	{
		vel.x = charSpeed;
	}
	else
	{
		vel.x = -charSpeed;
	}

	ProceedAnimFile();
}

void Enemy::JumpUpdate()
{
	ProceedAnimFile();
}

void Enemy::BubbleUpdate()
{
	vel.y = -0.5;		/// とりあえず、仮設定
	ProceedAnimFile();
}

void Enemy::DieUpdate()
{
	if (vel.y < charSpeed)
	{
		vel.y += fallAccel;
	}
	else
	{
		vel.x = 0;
		vel.y = charSpeed;
	}
	
	ProceedAnimFile();
}

bool Enemy::RiseCheck(const Rect& bRect)
{
	auto underBlock = CollisionDetector::UnderCollCheck(bRect, GetRect());
	if (underBlock)
	{
		groundLine = bRect.Top() - (size.y + bRect.size.height);
		riseFlag = false;
		return true;
	}
	return false;
}

void Enemy::SideCheck(const Rect & pRect, const Rect & wRect)
{
	auto hitCheck = CollisionDetector::CollCheck(GetRect(), pRect);
	auto wSideCheck = CollisionDetector::SideCollCheck(GetRect(), wRect);

	if (updater == &Enemy::BubbleUpdate &&
		updater != &Enemy::DieUpdate &&
		hitCheck)
	{
		if (wSideCheck)
		{
			if (wRect.Right() > GetRect().Left())
			{
				AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
				Die();
				turnFlag = true;
				vel.x = charSpeed;
				vel.y = -12.f;
			}
			else if (wRect.Left() < GetRect().Right())
			{
				AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
				Die();
				turnFlag = false;
				vel.x = -charSpeed;
				vel.y = -12.f;
			}
			else {}
		}
		if (pRect.Right() > GetRect().Left() &&
			pRect.Right() < GetRect().center.x)
		{
			vel.x = charSpeed;
		}
		else if (pRect.Left() < GetRect().Right() &&
				 pRect.Left() > GetRect().center.x)
		{
			vel.x = -charSpeed;
		}
		else {}
	}
	else
	{
		if (updater != &Enemy::DieUpdate) { vel.x = 0.f; }
	}
}

bool Enemy::UnderCheck(const Rect & pRect, const Input & p)
{
	auto blowDir = [=](const Rect& pRect)
	{
		if (pRect.Right() < GetRect().center.x)
		{
			return true;
			
		}
		else if (pRect.Left() > GetRect().center.x)
		{
			return false;
			
		}
		else {}
	};

	auto hitCheck	 = (CollisionDetector::CollCheck(GetRect(), pRect));
	auto underBubble = (CollisionDetector::UnderCollCheck(GetRect(), pRect));		// 泡の下側との当たり判定
	auto underPlayer = (CollisionDetector::UnderCollCheck(pRect, GetRect()));		// ﾌﾟﾚｲﾔｰの下側との当たり判定

	if (hitCheck && updater == &Enemy::BubbleUpdate)
	{
		if (p.IsPressing(PAD_INPUT_1))
		{
			/// 地上でﾎﾞﾀﾝを押し続けている時に泡に当たると、泡が割れる
			if (underBubble || (GetRect().Top() < pRect.center.y + (size.y / 4)))
			{
				if (blowDir(pRect))
				{
					AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
					Die();
					turnFlag = true;
					vel.x = charSpeed;
					vel.y = -12.f;
				}
				else
				{
					AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
					Die();
					turnFlag = false;
					vel.x = -charSpeed;
					vel.y = -12.f;
				}
				Die();
				return false;
			}
			/// ﾎﾞﾀﾝを押し続けていると、泡の上を飛ぶことができる
			if (underPlayer)
			{
				AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().bubble);
				return true;
			}
		}
		else
		{
			if (underPlayer)
			{
				if (blowDir(pRect))
				{
					AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
					Die();
					turnFlag = true;
					vel.x = charSpeed;
					vel.y = -12.f;
				}
				else
				{
					AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
					Die();
					turnFlag = false;
					vel.x = -charSpeed;
					vel.y = -12.f;
				}
				return false;
			}
		}

	}
	return false;return false;
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