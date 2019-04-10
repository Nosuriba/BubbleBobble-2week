#pragma once
#include "CharactorObject.h"

class Bubble :
	public CharactorObject
{
	Bubble();
	~Bubble();

	void Update();
	void Draw();
};