#include "HitCheck.h"


HitCheck::HitCheck()
{
}


HitCheck::~HitCheck()
{
}

bool HitCheck::HitPlayer(Rect player, Rect block)
{
	if (abs(player.center.x - block.center.x) < ((player.size.width / 2) + (block.size.width)) &&
		abs(player.center.y - block.center.y) < ((player.size.height / 2) + (block.size.height / 2)))
	{
		return true;
	}
	return false;
}
