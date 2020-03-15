#include <DxLib.h>
#include <cassert>
#include "CharactorObject.h"

CharactorObject::CharactorObject() : defSpeed(5.0f)
{

}

CharactorObject::CharactorObject(int groundLine) : defSpeed(5.0f)
{
}

CharactorObject::~CharactorObject()
{
}

void CharactorObject::ChangeAction(const char * name)
{
	frame = 0;
	nowCutIdx = 0;
	nowActionName = name;
}

void CharactorObject::Init(std::string actionName, const Vector2f & pos, const Vector2 & size)
{
	this->nowActionName = actionName;
	this->nowCutIdx		= 0;
	this->pos			= pos;
	this->size			= size;

}

bool CharactorObject::ProceedAnimFile()
{
	if ((signed)frame < actionData.actionInfo[nowActionName].cuts[nowCutIdx].duration)
	{
		frame++;
	}
	else
	{
		frame = 0;
		/// アニメーションの更新用
		if (nowCutIdx < (signed)actionData.actionInfo[nowActionName].cuts.size() - 1)
		{
			nowCutIdx++;
		}
		else
		{
			nowCutIdx = 0;

			/// アニメーションのリセット用
			if (actionData.actionInfo[nowActionName].isLoop)
			{
				nowCutIdx = 0;
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}

void CharactorObject::ReadActionFile(const char * actionPath)
{
	int h = DxLib::FileRead_open(actionPath, false);
	int hData;										// ファイル内容を格納するための変数

	float version = 0.f;
	DxLib::FileRead_read(&version, sizeof(version), h);

	DxLib::FileRead_read(&hData, sizeof(hData), h);

	std::string filePath = "";

	/// ファイルパスの読み込み
	filePath.resize(hData);
	DxLib::FileRead_read(&filePath[0], hData, h);

	std::string actPath = actionPath;
	int ipos = actPath.find_last_of('/') + 1;

	actionData.imgFilePath = actPath.substr(0, ipos) + filePath;

	int actionCnt = 0;
	DxLib::FileRead_read(&actionCnt, sizeof(actionCnt), h);

	for (int i = 0; i < actionCnt; ++i)
	{
		int actionNameSize;
		DxLib::FileRead_read(&actionNameSize, sizeof(actionNameSize), h);

		/// アクション名を取得
		std::string actionName;
		actionName.resize(actionNameSize);
		DxLib::FileRead_read(&actionName[0], actionName.size(), h);

		ObjectInfo actInfo;
		DxLib::FileRead_read(&actInfo.isLoop, sizeof(actInfo.isLoop), h);

		/// 切り取ったデータの取得
		int cutCount = 0;
		DxLib::FileRead_read(&cutCount, sizeof(cutCount), h);
		actInfo.cuts.resize(cutCount);

		for (int a = 0; a < cutCount; ++a)
		{
			DxLib::FileRead_read(&actInfo.cuts[a], (sizeof(actInfo.cuts[a]) - sizeof(actInfo.cuts[a].actRects)), h);

			// 矩形のデータを取得している
			int actRcCnt = 0;
			DxLib::FileRead_read(&actRcCnt, sizeof(actRcCnt), h);

			// 矩形のデータがない場合、最初に戻る
			if (actRcCnt == 0)
			{
				continue;
			}
			actInfo.cuts[a].actRects.resize(actRcCnt);
			DxLib::FileRead_read(&actInfo.cuts[a].actRects[0], sizeof(ActRect) * actRcCnt, h);
		}

		// アクションデータの登録
		actionData.actionInfo[actionName] = actInfo;
	}

	DxLib::FileRead_close(h);
}

Vector2f CharactorObject::GetPos() const
{
	return pos;
}

void CharactorObject::Draw(int img)
{
	auto& actInfo = actionData.actionInfo[nowActionName];
	auto& cut	  = actInfo.cuts[nowCutIdx];
	auto& rc	  = cut.rect;

	/// 中心点を変えないようにするためのもの
	int centerX = (turnFlag ? rc.Width() - cut.center.x : cut.center.x);

	/// pos.yの部分は修正後で考える
	DxLib::DrawRectRotaGraph2(pos.x, pos.y, rc.Left(), rc.Top(),
							  rc.Width(), rc.Height(), centerX - (size.x / 2), cut.center.y - (size.y / 2),
							  1.0f, 0.0, img, true, turnFlag);
}