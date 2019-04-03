#pragma once

#include <memory>
#include "Geometry.h"

#define LpGame (Game::GetInstance())

class Scene;

class Game
{
public:
	static Game & GetInstance()
	{
		return *s_Instance;
	}

	void Init();
	void Run();
	void End();

	const Vector2 & GetScreenSize();
	void ChangeScene(Scene * scene);
private:
	Game();
	~Game();
	struct GameDeleter
	{
		void operator()(Game * game) const
		{
			delete game;
		}
	};

	static std::unique_ptr<Game, GameDeleter> s_Instance;
	std::shared_ptr<Scene> scene;

	const Vector2 screenSize;
};

