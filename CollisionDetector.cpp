#include "CollisionDetector.h"


CollisionDetector::CollisionDetector()
{
}


CollisionDetector::~CollisionDetector()
{
}

const bool CollisionDetector::GroundCollCheck(Rect rcA, Rect rcB)
{
	return (rcA.Bottom() > rcB.Top() && rcA.Bottom() < rcB.Top() + (rcB.Height() / 2) && CollCheck(rcA, rcB));
}

const bool CollisionDetector::WallCollCheck(Rect rcA, Rect rcB)
{
	auto ar = rcA.Right();
	auto al = rcA.Left();
	auto br = rcB.Right();
	auto bl = rcB.Left();

	if (rcA.Right() > rcB.Left() - 1 && CollCheck(rcA, rcB))
	{
		auto debug = 0;
	}
	else if (rcA.Left() < rcB.Right() - 1 && CollCheck(rcA, rcB))
	{
		auto debug = 0;
	}

	return (rcA.Right() > (rcB.Left() - 1) && CollCheck(rcA, rcB)) || (rcA.Left() < (rcB.Right() - 1) && CollCheck(rcA, rcB));
}

const bool CollisionDetector::CollCheck(Rect rcA, Rect rcB)
{
	return abs(rcA.center.x - rcB.center.x) < (rcA.Width() + rcB.Width()) / 2
		&& abs(rcA.center.y - rcB.center.y) < (rcA.Height() + rcB.Height()) / 2;
}

