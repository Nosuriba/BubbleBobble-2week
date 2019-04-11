#pragma once
#include <vector>
#include "Geometry.h"

struct FMF
{
	int identifier;
	int size;
	int mapWidth;
	int mapHeight;
	char chipH;
	char chipW;
	char layerCount;
	char bitCount;
};

class Stage
{
public:
	Stage();
	~Stage();

	//ステージの範囲を渡す
	const Rect& GetStageRange()const;
	//現在の画面内の敵情報を渡す
	const std::vector<char> GetEnemyData(int minX, int maxX);
private:
	Rect stageRange;
	int readX;

	//ステージ情報
	FMF fmfData;
	//ステージのエネミー情報
	std::vector<char> enemyData;

};


