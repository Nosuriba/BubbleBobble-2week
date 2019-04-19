#pragma once

#include "Geometry.h"

class CollisionDetector
{
public:
	CollisionDetector();
	~CollisionDetector();

	const static bool UnderCollCheck(Rect rcA, Rect rcB);		// ���̓����蔻��
	const static bool SideCollCheck(Rect rcA, Rect rcB);		// �ǂ̓����蔻��
	const static bool CollCheck(Rect rcA, Rect rcB);			// ��`�̓����蔻��
};

