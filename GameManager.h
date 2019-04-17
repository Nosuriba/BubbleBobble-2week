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
	void CreateStage();		// ｽﾃｰｼﾞの生成をしている
	void CreateBubble();
	void WallInstance();	// 壁の設置
	void BlockInstance();	// ﾌﾞﾛｯｸの設置

	void PlayerCollision();	// ﾌﾟﾚｲﾔｰの当たり判定検出用
	void BubbleCollision(const Input & p);	// 泡の当たり判定検出用

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

