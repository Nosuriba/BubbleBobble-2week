#include <DxLib.h>
#include "stdio.h"
#include "Stage.h"



Stage::Stage()
{
	/// 読み込むステージファイルの指定
	int h = DxLib::FileRead_open("stage/stage1.fmf", false);

	DxLib::FileRead_read(&fmfData, sizeof(fmfData), h);

	/// 縦横のﾏｯﾌﾟﾃﾞｰﾀを読み込んでいる
	std::vector<char> tmp;
	tmp.resize(fmfData.mapWidth * fmfData.mapHeight);
	DxLib::FileRead_read(&tmp[0], tmp.size(), h);

	DxLib::FileRead_close(h);

	/// ステージの大きさと中心を設定している
	stageRange.size.width  = fmfData.mapWidth    * fmfData.chipW;			/// ステージサイズは2倍にしなくていいのでは...
	stageRange.size.height = fmfData.mapHeight   * fmfData.chipH;
	stageRange.center.x	   = stageRange.size.width  / 2;
	stageRange.center.y    = stageRange.size.height / 2;

	/// ﾏｯﾌﾟのｻｲｽﾞ分、敵のﾃﾞｰﾀｻｲｽﾞを確保している
	enemyData.resize(fmfData.mapWidth * fmfData.mapHeight);

	for (int y = 0; y < fmfData.mapHeight; y++)
	{
		for (int x = 0; x < fmfData.mapWidth; x++)
		{
			enemyData[x * fmfData.mapHeight + y] = tmp[y * fmfData.mapWidth + x];
		}
	}

	readX = 0;
}


Stage::~Stage()
{
}

const Rect & Stage::GetStageRange() const
{
	return stageRange;
}

const std::vector<char> Stage::GetEnemyData(int minX, int maxX)
{
	/// 敵ﾃﾞｰﾀを最初と最後を登録している
	int L = max((minX / fmfData.chipW * 2), readX);
	int R = maxX / (fmfData.chipW * 2) + 1;

	/// 敵の情報がない場合
	if (R <= readX)
	{
		return std::vector<char>();
	}

	/// 敵の情報を登録している
	auto idxL = L * fmfData.mapHeight;
	auto idxR = min(R * fmfData.mapHeight, enemyData.size());
	auto begin = enemyData.begin() + idxL;
	auto end = min(enemyData.begin() + idxR, enemyData.end());
	readX = R;
	return std::vector<char>(begin, end);
}