#include "CollisionDetector.h"


CollisionDetector::CollisionDetector()
{
}


CollisionDetector::~CollisionDetector()
{
}

const bool CollisionDetector::CollisionCheck(Rect rcA, Rect rcB)
{
	return abs(rcA.center.x - rcB.center.x) < (rcA.Width() + rcB.Width()) / 2
		&& abs(rcA.center.y - rcB.center.y) < (rcA.Height() + rcB.Height()) / 2;
}

