#pragma once
#include <vector>
#include "Scene.h"

class Bubble;

class TitleScene :
	public Scene
{
public:
	TitleScene();
	~TitleScene();
	
	void Init();
	void Update(const Input & p);
private:
	std::vector<std::shared_ptr<Bubble>> bubbles;

	Vector2f pos;
	Vector2f vel;
	int invCnt;			// 泡を出す時間の間隔
	int dispCnt;
};

