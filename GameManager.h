#pragma once

#include <memory>
#include <vector>
#include "Geometry.h"

class Input;
class Player;
class Stage;
class Wall;
class Block;
class Bubble;

class GameManager
{
public:
	GameManager();
	~GameManager();

	void Init();
	void Update(const Input & p);
private:
	void CreateStage();		
	void CreateBubble();	
	void WallInstance();	
	void BlockInstance();	

	void PlayerCollision();	
	void BubbleCollision(const Input & p);	

	std::shared_ptr<Player> player;
	std::shared_ptr<Stage>  stage;

	std::vector<std::shared_ptr<Wall>>   walls;
	std::vector<std::shared_ptr<Block>>  blocks;
	std::vector<std::shared_ptr<Bubble>> bubbles;
	
	const Vector2 blockMax;
	const int blockSize;
	const int playerSize;
	const int bubbleSize;
	const int wallSize;
	const int wallMax;
};

