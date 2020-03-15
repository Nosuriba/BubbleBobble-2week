#pragma once
#include "CharactorObject.h"

class Input;

class Bubble :
	public CharactorObject
{
public:
	Bubble();
	Bubble(const bool& bubbleDir);
	~Bubble();

	void Update();
	void Draw();

	// 泡を割る状態にする
	void ChangePop();

	void HitAcross(const Rect & pRect, const Rect& wRect);

	/// 泡以外のオブジェクトとの当たり判定
	bool HitPlayer(const Rect& pRect, const Input& p);
	bool HitEnemy(const Rect& rcB);
	bool HitObject(const Rect& objRect);
	bool HitBubble(const Rect& bblRect);

	// 上の確認
	bool CeilCheck();

	// 削除するかの確認
	bool IsDelete();

	// 泡同士の接触が行われたときの移動
	void MoveContact(const Rect& bblRect);

	const Vector2f& GetPos();
	Rect GetRect();
	Rect ShotGetRect();
private:
	void Shot();		// 泡を吐いた時
	void Floating();	// 泡が浮いている時	
	void Pop();

	void ShotUpdate();
	void FloatingUpdate();
	void PopUpdate();

	// 横の確認
	void SideCheck(const Rect & pRect, const Rect& wRect);	

	// 下の確認
	bool UnderCheck(const Rect& pRect, const Input& p);		
	
	void DebugDraw();

	void (Bubble::*updater)();

	bool isGame;		// true:ゲーム中, false:ゲーム中でない
	bool bubbleDir;		// true:左方向, false:右方向
	bool isDelete;		// true:削除, false:削除しない

	int bblImage;		// 泡の画像ID取得用
	int invCnt;
};