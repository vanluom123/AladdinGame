#include "Game.h"
#include "GameGlobal.h"
#include "../GameControllers/SceneManager.h"
#include "../Scenes/DemoScene.h"

#include "../GameControllers/SpriteManager.h"
Game::Game(int fps)
{
	this->mFPS = fps;
	SpriteManager::getInstance()->loadResource();
	SceneManager::GetInstance()->ReplaceScene(new DemoScene());
	this->InitLoop();
}

Game::~Game()
{
}

void Game::Update(float dt)
{
	SceneManager::GetInstance()->Update(dt);

	this->Render();
}

void Game::Render()
{
	auto device = GameGlobal::GetCurrentDevice();

	device->Clear(0, NULL, D3DCLEAR_TARGET, SceneManager::GetInstance()->GetCurrentScene()->GetBackcolor(), 0.0f, 0);

	if (SUCCEEDED(device->BeginScene()))
	{
		//bat dau ve
		GameGlobal::GetCurrentSpriteHandler()->Begin(D3DXSPRITE_ALPHABLEND);
		SceneManager::GetInstance()->GetCurrentScene()->Draw();
		GameGlobal::GetCurrentSpriteHandler()->End();
		
		device->EndScene();
	}

	device->Present(0, 0, 0, 0);
}

void Game::InitLoop()
{
	MSG msg;

	float tickPerFrame = 1.0f / mFPS, delta = 0;

	while (GameGlobal::isGameRunning)
	{
		GameTime::GetInstance()->StartCounter();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		delta += GameTime::GetInstance()->GetCouter();

		if (delta >= tickPerFrame)
		{
			Update((delta));
			delta = 0;
		}
		else
		{
			Sleep(tickPerFrame - delta);
			delta = tickPerFrame;
		}
	}
}