#pragma once

#include <vector>
#include <map>
#include "../Geometry.h"

enum class RectType
{
	ANCHOR,		// �A���J�[
	ATTACK,		// �U����`
	DAMAGE,		// �����`
	MAX
};

struct ActRect
{
	RectType rt;	// ��`�̏��
	Rect     rc;	// ��`�̕��ƍ���
};

struct CutInfo
{
	Rect rect;			// �؂����`
	Vector2 center;		// ���S�_
	int duration;		// �\������
	std::vector<ActRect> actRects;	// �A�N�V������`
};

struct ObjectInfo
{
	bool isLoop;				// �A�j���[�V�����̃��[�v����	
	std::vector<CutInfo> cuts;	// �؂���������ۑ�����z��		
};

struct ObjectData
{
	std::string imgFilePath;						// �摜�̃t�@�C���p�X
	std::map<std::string, ObjectInfo> actionInfo;	// �A�N�V�������
};

class Object
{
public:
	Object()
	{

	}
	~Object()
	{

	}
};

