#pragma once
#include "Scene.h"

class Object;

class MainScene :
	public Scene
{
public:
	MainScene();
	~MainScene();

	void Init();
	void Update(const Input & p);

	std::shared_ptr<Object> player;

};

