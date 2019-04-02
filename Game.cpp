#include <DxLib.h>
#include "Game.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/TitleScene.h"

std::unique_ptr<Game, Game::GameDeleter> Game::s_Instance(new Game());

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init()
{
	DxLib::SetGraphMode(640, 480, 32);
	DxLib::ChangeWindowMode(true);
	DxLib::SetWindowText("–¼‘O‚Í‚È‚¢‚¼");
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
		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			break;
		}

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

void Game::ChangeScene(Scene * scene)
{
	this->scene.reset(scene);
}


