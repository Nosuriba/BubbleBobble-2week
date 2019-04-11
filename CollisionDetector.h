#pragma once

#include "Geometry.h"

class CollisionDetector
{
public:
	CollisionDetector();
	~CollisionDetector();

	const static bool GroundCollCheck(Rect rcA, Rect rcB);		// ���̓����蔻��
	const static bool WallCollCheck(Rect rcA, Rect rcB);		// �ǂ̓����蔻��
	const static bool CollCheck(Rect rcA, Rect rcB);			// ��`�̓����蔻��
};

