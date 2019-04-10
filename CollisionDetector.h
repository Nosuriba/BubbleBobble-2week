#pragma once

#include "Geometry.h"

class CollisionDetector
{
public:
	CollisionDetector();
	~CollisionDetector();

	const static bool CollisionCheck(Rect rcA, Rect rcB);
};

