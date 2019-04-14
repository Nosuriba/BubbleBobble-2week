#pragma once

#include <memory>
#include <vector>
#include "Geometry.h"

class Input;
class Player;
class Stage;
class Wall;
class Block;

class GameManager
{
public:
	GameManager();
	~GameManager();

	void Init();
	void Update(const Input & p);
private:
	void CreateBlock();				// Ω√∞ºﬁÃﬁ€Ø∏ÇÃê∂ê¨ÇÇµÇƒÇ¢ÇÈ

	std::shared_ptr<Player> player;
	std::shared_ptr<Stage> stage;
	std::vector<std::shared_ptr<Wall>> walls;

	std::vector<std::shared_ptr<Block>> blocks;
	
	const Vector2 blockMax;
	const int blockSize;
	const int playerSize;
	const int wallSize;
	const int wallMax;

};

