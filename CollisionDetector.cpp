#include "CollisionDetector.h"


CollisionDetector::CollisionDetector()
{
}


CollisionDetector::~CollisionDetector()
{
}

const bool CollisionDetector::UnderCollCheck(Rect rcA, Rect rcB)
{
	return (rcA.Bottom() >= rcB.Top() && rcA.Bottom() <= (rcB.Top() + (rcB.Height() / 3)) && CollCheck(rcA, rcB));
}

const bool CollisionDetector::SideCollCheck(Rect rcA, Rect rcB)
{	
	return ((rcA.Right() > rcB.Left() && CollCheck(rcA, rcB)) ||
			(rcA.Left() < rcB.Right() && CollCheck(rcA, rcB)));
}

const bool CollisionDetector::CollCheck(Rect rcA, Rect rcB)
{
	return abs(rcA.center.x - rcB.center.x) < (rcA.Width() + rcB.Width()) / 2
		&& abs(rcA.center.y - rcB.center.y) < (rcA.Height() + rcB.Height()) / 2;
}

