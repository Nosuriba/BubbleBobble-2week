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

	//�X�e�[�W�͈̔͂�n��
	const Rect& GetStageRange()const;
	//���݂̉�ʓ��̓G����n��
	const std::vector<char> GetEnemyData(int minX, int maxX);
private:
	Rect stageRange;
	int readX;

	//�X�e�[�W���
	FMF fmfData;
	//�X�e�[�W�̃G�l�~�[���
	std::vector<char> enemyData;

};


