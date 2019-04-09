#pragma once

#include "Geometry.h"

class HitCheck
{
public:
	HitCheck();
	~HitCheck();

	bool HitPlayer(Rect player, Rect block);
};

