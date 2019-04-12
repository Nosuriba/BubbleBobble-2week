#include <DxLib.h>
#include "stdio.h"
#include "Stage.h"



Stage::Stage()
{
	/// �ǂݍ��ރX�e�[�W�t�@�C���̎w��
	int h = DxLib::FileRead_open("stage/stage1.fmf", false);

	DxLib::FileRead_read(&fmfData, sizeof(fmfData), h);

	/// �c����ϯ���ް���ǂݍ���ł���
	std::vector<char> tmp;
	tmp.resize(fmfData.mapWidth * fmfData.mapHeight);
	DxLib::FileRead_read(&tmp[0], tmp.size(), h);

	DxLib::FileRead_close(h);

	/// �X�e�[�W�̑傫���ƒ��S��ݒ肵�Ă���
	stageRange.size.width  = fmfData.mapWidth    * fmfData.chipW;			/// �X�e�[�W�T�C�Y��2�{�ɂ��Ȃ��Ă����̂ł�...
	stageRange.size.height = fmfData.mapHeight   * fmfData.chipH;
	stageRange.center.x	   = stageRange.size.width  / 2;
	stageRange.center.y    = stageRange.size.height / 2;

	/// ϯ�߂̻��ޕ��A�G���ް����ނ��m�ۂ��Ă���
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
	/// �G�ް����ŏ��ƍŌ��o�^���Ă���
	int L = max((minX / fmfData.chipW * 2), readX);
	int R = maxX / (fmfData.chipW * 2) + 1;

	/// �G�̏�񂪂Ȃ��ꍇ
	if (R <= readX)
	{
		return std::vector<char>();
	}

	/// �G�̏���o�^���Ă���
	auto idxL = L * fmfData.mapHeight;
	auto idxR = min(R * fmfData.mapHeight, enemyData.size());
	auto begin = enemyData.begin() + idxL;
	auto end = min(enemyData.begin() + idxR, enemyData.end());
	readX = R;
	return std::vector<char>(begin, end);
}