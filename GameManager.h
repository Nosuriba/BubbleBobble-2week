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
	void CreateStage();		// �ð�ނ̐��������Ă���
	void CreateBubble();
	void WallInstance();	// �ǂ̐ݒu
	void BlockInstance();	// ��ۯ��̐ݒu

	void PlayerCollision();	// ��ڲ԰�̓����蔻�茟�o�p
	void BubbleCollision(const Input & p);	// �A�̓����蔻�茟�o�p

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

