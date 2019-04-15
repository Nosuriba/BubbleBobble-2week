#include "CollisionDetector.h"


CollisionDetector::CollisionDetector()
{
}


CollisionDetector::~CollisionDetector()
{
}

const bool CollisionDetector::GroundCollCheck(Rect rcA, Rect rcB)
{
	return (rcA.Bottom() >= rcB.Top() && rcA.Bottom() <= (rcB.Top() + (rcB.Height() / 4)) && CollCheck(rcA, rcB));
}

const bool CollisionDetector::WallCollCheck(Rect rcA, Rect rcB)
{	
	auto RL = (rcA.Right() > rcB.Left());
	auto LR = rcA.Left() < rcB.Right();
	auto Coll = CollCheck(rcA, rcB);

	return ((rcA.Right() > rcB.Left() && CollCheck(rcA, rcB)) ||
			(rcA.Left() < rcB.Right() && CollCheck(rcA, rcB)));
}

const bool CollisionDetector::CollCheck(Rect rcA, Rect rcB)
{
	return abs(rcA.center.x - rcB.center.x) < (rcA.Width() + rcB.Width()) / 2
		&& abs(rcA.center.y - rcB.center.y) < (rcA.Height() + rcB.Height()) / 2;
}

