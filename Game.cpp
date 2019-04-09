#include <DxLib.h>
#include "Game.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/TitleScene.h"

std::unique_ptr<Game, Game::GameDeleter> Game::s_Instance(new Game());

Game::Game() : screenSize(640, 480)
{
}

Game::~Game()
{
}

void Game::Init()
{
	DxLib::SetGraphMode(screenSize.x, screenSize.y, 32);
	DxLib::ChangeWindowMode(true);
	DxLib::SetWindowText("Bubble Bubble");
	if (DxLib_Init() == -1)
	{
		return;
	}
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	ChangeScene(new TitleScene());
}

void Game::Run()
{
	Input input;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClsDrawScreen();

		input.Update();

		scene->Update(input);

		ScreenFlip();
	}
}

void Game::End()
{
	DxLib_End();
}

const Vector2 & Game::GetScreenSize()
{
	return screenSize;
}

void Game::ChangeScene(Scene * scene)
{
	this->scene.reset(scene);
}


