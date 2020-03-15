#include "GameManager.h"
#include "Input.h"
#include "Game.h"
#include "Stage.h"
#include "AudioMng.h"
#include "Object/Player.h"
#include "Object/Enemy.h"
#include "Object/Bubble.h"
#include "StageObject/Block.h"
#include "StageObject/Wall.h"

const Vector2 blockMax = Vector2(28,26);
const int blockSize	   = 24;
const int charSize	   = 48;
const int bubbleSize   = 48;
const int wallSize     = 48;
const int wallMax	   = 26;

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	/// �X�e�[�W�̐���
	stage = std::make_shared<Stage>();

	/// �v���C���[�̐���
	player = std::make_shared<Player>(Game::GetInstance().GetScreenSize().y - blockSize);
	player->Init("idle", Vector2f(charSize * 2, Game::GetInstance().GetScreenSize().y - (charSize + blockSize - 1)), Vector2(charSize, charSize));

	/// �G�̐���
	for (int i = 0; i < 3; ++i)
	{
		enemies.push_back(std::make_shared<Enemy>(Game::GetInstance().GetScreenSize().y - blockSize));
		enemies[i]->Init("run", Vector2f(Game::GetInstance().GetScreenSize().x / 2 - blockSize, (i * charSize) - (charSize * 3)),
								Vector2(charSize, charSize));
	}
	CreateStage();
}

void GameManager::CreateStage()
{
	/// �Q�[�����̃I�u�W�F�N�g����
	WallInstance();
	BlockInstance();
}

void GameManager::CreateBubble()
{
	if (player->IsShot())
	{
		bubbles.push_back(std::make_shared<Bubble>(player->GetTurnFlag()));
		bubbles[bubbles.size() - 1]->Init("shot", player->GetPos(), Vector2(bubbleSize, bubbleSize));
	}
}

void GameManager::WallInstance()
{
	for (auto i = 0; i < wallMax; i++)
	{
		/// �ǂ̍��W������
		auto pos = Vector2f((Game::GetInstance().GetScreenSize().x - wallSize) * (i / (wallMax / 2)),
							 wallSize + (wallSize * (i % (wallMax / 2))));

		/// �ǂ̐���
		walls.push_back(std::make_shared<Wall>());
		walls[i]->Init("resource/Image/wall.png", pos, Vector2(2, 8), Vector2(0, 0), Vector2(wallSize, wallSize));
	}
}

void GameManager::BlockInstance()
{
	int blockCnt = 0;
	int cnt		 = 0;							/// �u���b�N�̔Ԓn�A�N�Z�X�p
	std::string name[] = { "0", "2" };			/// ����̃X�e�[�W�`�b�v��ǂݍ��ނ悤�ɂ��Ă���(���ݒ�)
	auto blockData = stage->GetEnemyData(0, (stage->GetStageRange().Width() * stage->GetStageRange().Height()));
	auto mapSize = stage->GetStageRange();

	for (auto& tmp : blockData)
	{
		if (name[tmp] == "0")
		{
			/// �u���b�N�f�[�^�̍��W�ǂݍ���
			auto pos = Vector2f(wallSize + (blockSize * (blockCnt / blockMax.y)),
								wallSize + (blockSize * (blockCnt % blockMax.y)));

			/// �u���b�N�̐���
			blocks.push_back(std::make_shared<Block>());
			blocks[cnt]->Init("resource/Image/block.png", pos, Vector2(2, 8), Vector2(0, 0), Vector2(blockSize, blockSize));
			cnt++;
		}
		blockCnt++;
	}
}

void GameManager::PlayerCollision()
{
	/// �v���C���[�̓����蔻��
	for (auto itr : walls)
	{
		/// �ǂƂ̓����蔻��
		if (player->IsHitWall(itr->GetRect()))
		{
			break;
		}
	}

	for (auto itr : blocks)
	{
		/// �u���b�N�Ƃ̓����蔻��
		if (player->isGround(itr->GetRect()))
		{
			break;
		}
	}

	for (auto itr : enemies)
	{
		/// �G�Ƃ̓����蔻��
		if (player->IsHitEnemy(itr->GetRect(), itr->IsAlive()))
		{
			break;
		}
	}
}

void GameManager::EnemyCollision(const Input& p)
{
	//// �G�̓����蔻��
	for (auto itr : enemies)
	{
		for (auto wall : walls)
		{
			/// �v���C���[�Ƃ̓����蔻��
			if (itr->HitPlayer(player->GetRect(), wall->GetRect(), p))
			{
				player->IsStepBubble();
			}

			/// �ǂƂ̓����蔻��
			if (itr->HitWall(wall->GetRect()))
			{
				break;
			}
			itr->DieControl(wall->GetRect());
		}

		for (auto block : blocks)
		{
			/// �v���C���[�̏�����Ƃ̓����蔻��
			if (itr->UpperCheck(player->GetRect(), block->GetRect()))
			{
				break;
			}

			/// ���Ƃ̓����蔻��
			if (itr->HitGround(player->GetRect(), block->GetRect()))
			{
				break;
			}
			itr->DieControl(block->GetRect());
		}

		for (auto bubble = bubbles.begin(); bubble != bubbles.end(); bubble++)
		{
			/// �A�Ƃ̓����蔻��
			if (itr->HitBubble((*bubble)->GetRect(), (*bubble)->HitEnemy(itr->GetRect())))
			{
				bubbles.erase(bubble);
				break;
			}
		}
	}
}

void GameManager::BubbleCollision(const Input& p)
{
	//// �A�̓����蔻��
	for (auto itr = bubbles.begin(); itr != bubbles.end(); ++itr)
	{
		/// �v���C���[�Ƃ̓����蔻��
		if ((*itr)->HitPlayer(player->GetRect(), p))
		{
			auto debug = (*itr)->HitPlayer(player->GetRect(), p);
			player->IsStepBubble();
		}
		
		for (auto bubble = bubbles.begin(); bubble != bubbles.end(); ++bubble)
		{
			///	�A������邩�̔���
			if ((*itr)->HitBubble((*bubble)->GetRect())) 
			{
				(*bubble)->ChangePop();
			}

			/// �����Ă���A���m�������������̔���
			if (itr != bubble)
			{
				/// �A���m���ڐG�������A�ړ�����
				(*itr)->MoveContact((*bubble)->GetRect());
			}
		}

		for (auto wall : walls)
		{
			/// �ǂƂ̓����蔻��
			(*itr)->HitAcross(player->GetRect(), wall->GetRect());
			if ((*itr)->HitObject(wall->GetRect()))
			{
				return;
			}
		}

		for (auto block : blocks)
		{
			/// �u���b�N�Ƃ̓����蔻��
			if ((*itr)->HitObject(block->GetRect()))
			{
				return;
			}
		}
	}
}

void GameManager::Update(const Input & p)
{
	/// �I�u�W�F�N�g�̕`��
	for (auto itr : walls)
	{
		itr->Draw();
	}
	for (auto itr : blocks)
	{
		itr->Draw();
	}

	/// �L�����N�^�[�̓����蔻��
	PlayerCollision();
	EnemyCollision(p);

	/// �A�̓����蔻��
	BubbleCollision(p);

	/// �v���C���[�̍X�V
	player->Update(p);
	player->Draw();

	/// �G�̍X�V
	for (auto enemy : enemies)
	{
		enemy->Update();
		enemy->Draw();
	}

	/// �A�̍X�V
	for (unsigned int i = 0; i < bubbles.size(); ++i)
	{
		if (bubbles[i]->IsDelete())
		{
			bubbles.erase(bubbles.begin() + i);
			continue;
		}
		bubbles[i]->Update();
		bubbles[i]->Draw();
			
	}

	/// �A�̐���
	CreateBubble();

	/// �A����萔�𒴂�����A�ŏ��ɍ��ꂽ�A���폜����
	if (bubbles.size() > 10)
	{
		bubbles[0]->ChangePop();
	}

}
