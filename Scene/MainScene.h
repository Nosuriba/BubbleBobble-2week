#pragma once
#include "Scene.h"

class Player;
class Block;
class Wall;

class MainScene :
	public Scene
{
public:
	MainScene();
	~MainScene();

	void Init();
	void Update(const Input & p);

	std::shared_ptr<Player> player;
	std::shared_ptr<Block> block;		/// vector‚É•Ï‚í‚é—\’è
	std::vector<std::shared_ptr<Wall>> wall;

	const int wallCnt;

};

