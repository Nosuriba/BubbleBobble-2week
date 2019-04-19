#pragma once

#include "Geometry.h"

class CollisionDetector
{
public:
	CollisionDetector();
	~CollisionDetector();

	const static bool UnderCollCheck(Rect rcA, Rect rcB);		// °‚Ì“–‚½‚è”»’è
	const static bool SideCollCheck(Rect rcA, Rect rcB);		// •Ç‚Ì“–‚½‚è”»’è
	const static bool CollCheck(Rect rcA, Rect rcB);			// ‹éŒ`‚Ì“–‚½‚è”»’è
};

