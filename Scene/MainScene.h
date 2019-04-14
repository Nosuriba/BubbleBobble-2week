#pragma once
#include <memory>
#include <vector>
#include "../Geometry.h"
#include "Scene.h"

class GameManager;

class MainScene :
	public Scene
{
public:
	MainScene();
	~MainScene();

	void Init();
	void Update(const Input & p);
private:
	std::shared_ptr<GameManager> game;
};

