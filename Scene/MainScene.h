#pragma once
#include <vector>
#include "../Geometry.h"
#include "Scene.h"

class Player;
class Block;
class Stage;
class Wall;
class Block;

using VEC2_BLOCK = std::vector<std::vector<std::shared_ptr<Block>>>;

class MainScene :
	public Scene
{
public:
	MainScene();
	~MainScene();

	void Init();
	void Update(const Input & p);

	std::shared_ptr<Player> player;
	// std::shared_ptr<Block> block;		/// vector‚É•Ï‚í‚é—\’è
	std::shared_ptr<Stage> stage;
	std::vector<std::shared_ptr<Wall>> wall;

	std::vector<std::shared_ptr<Block>> blocks;
	//VEC2_BLOCK blocks;

	const Vector2 chipMax;
	const int chipSize;
	const int playerSize;
	const int wallSize;
	const int wallMax;

};

