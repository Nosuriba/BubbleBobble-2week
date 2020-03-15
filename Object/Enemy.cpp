#include <DxLib.h>
#include "Enemy.h"
#include "../Game.h"
#include "../Input.h"
#include "../CollisionDetector.h"
#include "../AudioMng.h"

const float fallAccel = 0.3f;

Enemy::Enemy()
{
}

Enemy::Enemy(int groundLine)
{
	Run();

	/// 画像IDの初期化S
	nowCutIdx = 0;
	ReadActionFile("Action/enemy.act");
	enemyImg = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	/// 床の初期化
	this->groundLine = groundLine;

	/// 座標と速度の初期化
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
bool Enemy::IsAlive()
{
	/// 敵が生存状態かの判定用
	return (updater != &Enemy::BubbleUpdate && updater != &Enemy::DieUpdate);
}

bool Enemy::HitPlayer(const Rect & pRect, const Rect& wRect, const Input& p)
{
	IsSide(pRect, wRect);		// 敵の横側に当たったか調べている
	return IsUnder(pRect, p);	// 
}

bool Enemy::HitBubble(const Rect & bRect, const bool& bblCheck)
{
	auto hitCheck = CollisionDetector::CollCheck(GetRect(), bRect);

	/// ｼｮｯﾄの状態の泡に当たった時、敵が泡状態になる
	if (updater != &Enemy::BubbleUpdate && 
		updater != &Enemy::DieUpdate	&& 
		hitCheck && bblCheck)
	{
		Bubble();
		return true;
	}
	return false;
}

bool Enemy::HitWall(const Rect & wRect)
{
	auto isHitWall = CollisionDetector::CollCheck(GetRect(), wRect);
	if (isHitWall && updater != &Enemy::DieUpdate)
	{
		/// 敵の方向転換
		turnFlag = !turnFlag;
		vel.x	 = (vel.x == defSpeed ? -defSpeed : defSpeed);
		return true;
	}

	return false;
}

bool Enemy::HitGround(const Rect& pRect, const Rect & bRect)
{
	auto isUnder = CollisionDetector::UnderCollCheck(GetRect(), bRect);
	
	if (updater == &Enemy::RunUpdate)
	{
		if (!riseFlag)
		{
			/// 落下中にブロックの上に乗った時の処理
			if (isUnder && GetRect().Bottom() > size.y * 3)
			{
				vel.y = 0;
				groundLine = bRect.Top() + 1;		
				return true;
			}
			vel.y = 5.0f;
			groundLine = Game::GetInstance().GetScreenSize().y + (size.y * 2);
		}
	}
	return false;
}

bool Enemy::UpperCheck(const Rect& pRect, const Rect & bRect)
{
	if (updater == &Enemy::RunUpdate)
	{
		/// 頭上のプレイヤーを探索する処理
		if (pos.y > pRect.Bottom()				&&
			(int)pos.x == (int)bRect.center.x	&&
			bRect.Bottom() > size.y * 3			&& 
			bRect.Bottom() < pos.y)
		{
			turnFlag   = (GetRect().center.x < pRect.center.x ? true : false);
			groundLine = 0;
			riseFlag   = true;
			Idle();
		}
		return false;
	}

	/// プレイヤーを見つけた時、敵を上昇させる
	if (updater == &Enemy::IdleUpdate && waitCnt < 0)
	{
		if (riseFlag)
		{
			vel.y = -defSpeed;
			if (IsRiseUnder(bRect))
			{
				/// 上昇中に当たったﾌﾞﾛｯｸの上側を地面にしている
				turnFlag	= (GetRect().center.x > pRect.center.x ? false : true);
				groundLine  = bRect.Top() + 1;
				riseFlag    = false;
				return true;
			}
		}
		else
		{
			/// 敵の下側が、地面に当たったら落下を止める
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
	auto selHitCheck = [&](const Rect& objRect)
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
	/// 敵のサイズ確認用
	auto isEnemySize = (size.y == objRect.size.height ? true : false);

	if (updater == &Enemy::DieUpdate)
	{
		if (selHitCheck(objRect) && !isEnemySize && vel.y == defSpeed)
		{
			/// 床に当たった時、死亡状態にする
			vel		= Vector2f(0, 0);
			dieFlag = true;
			return;
		}

		if (selHitCheck(objRect) && isEnemySize)
		{
			/// 壁に当たった時に敵を反射させている
			pos.x = (vel.x == defSpeed ? objRect.Left() - size.x : objRect.Right());	
			vel.x = (vel.x == defSpeed ? -defSpeed : defSpeed);
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
		return;
	}
	turnFlag = ((waitCnt / 20) % 2 ? true : false);
	
	ProceedAnimFile();
}

void Enemy::RunUpdate()
{
	if (vel.y == 0.f)
	{
		if (turnFlag)
		{
			vel.x = defSpeed;
		}
		else
		{
			vel.x = -defSpeed;
		}
	}
	

	ProceedAnimFile();
}

void Enemy::JumpUpdate()
{
	ProceedAnimFile();
}

void Enemy::BubbleUpdate()
{
	vel.y = -0.5;		
	ProceedAnimFile();
}

void Enemy::DieUpdate()
{
	if (!dieFlag)
	{
		if (vel.y < defSpeed)
		{
			vel.y += fallAccel;
		}
		else
		{
			vel.x = 0;
			vel.y = defSpeed;
		}
	}
	
	
	ProceedAnimFile();
}

bool Enemy::IsRiseUnder(const Rect& bRect)
{
	auto isUnder = CollisionDetector::UnderCollCheck(bRect, GetRect());
	if (isUnder)
	{
		return true;
	}
	return false;
}

void Enemy::IsSide(const Rect & pRect, const Rect & wRect)
{
	auto isHit   = CollisionDetector::CollCheck(GetRect(), pRect);
	auto isWallSide = CollisionDetector::SideCollCheck(GetRect(), wRect);

	if (updater == &Enemy::BubbleUpdate &&
		updater != &Enemy::DieUpdate &&
		isHit)
	{
		if (isWallSide)
		{
			if (wRect.Right() > GetRect().Left())
			{
				/// 死亡して、右方向に飛ぶ
				AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
				Die();
				turnFlag = true;
				vel.x = defSpeed;
				vel.y = -(defSpeed * 2);
			}
			else if (wRect.Left() < GetRect().Right())
			{
				/// 死亡して、左方向に飛ぶ
				AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
				Die();
				turnFlag = false;
				vel.x = -defSpeed;
				vel.y = -(defSpeed * 2);
			}
			else {}
		}
	}
	else
	{
		if (updater != &Enemy::DieUpdate) { vel.x = 0.f; }
	}
}

bool Enemy::IsUnder(const Rect & pRect, const Input & p)
{
	auto isDir		 = (pRect.Right() < GetRect().center.x);						// true:右に当たった, false:左に当たった
	auto isHit		 = (CollisionDetector::CollCheck(GetRect(), pRect));
	auto underBubble = (CollisionDetector::UnderCollCheck(GetRect(), pRect));		// 泡の下側との当たり判定
	auto underPlayer = (CollisionDetector::UnderCollCheck(pRect, GetRect()));		// ﾌﾟﾚｲﾔｰの下側との当たり判定

	if (isHit && updater == &Enemy::BubbleUpdate)
	{
		/// ジャンプボタンを押しっぱなしの時
		if (p.IsPressing(PAD_INPUT_1))
		{
			/// プレイヤーが地上で泡状態の敵に当たると。泡が割れる
			if (underBubble || (GetRect().Top() < pRect.center.y + (size.y / 4)))
			{
				if (isDir)
				{
					AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
					Die();
					turnFlag = true;
					vel.x = defSpeed;
					vel.y = -(defSpeed * 2);
				}
				else
				{
					AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
					Die();
					turnFlag = false;
					vel.x = -defSpeed;
					vel.y = -(defSpeed * 2);
				}
				return false;
			}
			/// ボタンを押し続けていると、泡の上を飛ぶことができる
			if (underPlayer)
			{
				return true;
			}
		}
		else
		{
			/// ジャンプボタンを押していない状態の時
			if (underPlayer)
			{
				if (isDir)
				{
					/// 死亡して、右方向に飛ぶ
					AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
					Die();
					turnFlag = true;
					vel.x = defSpeed;
					vel.y = -(defSpeed * 2);
				}
				else
				{
					/// 死亡して、左方向に飛ぶ
					AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().hit);
					Die();
					turnFlag = false;
					vel.x = -defSpeed;
					vel.y = -(defSpeed * 2);
				}
				return false;
			}
		}
	}
	return false;
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