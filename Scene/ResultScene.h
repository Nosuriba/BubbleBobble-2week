#pragma once
#include "Scene.h"

class ResultScene :
	public Scene
{
public:
	ResultScene();
	~ResultScene();

	void Init();
	void Update(const Input & p);
private:
	int dispCnt;
};

