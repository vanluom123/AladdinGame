#include "Scene.h"


Scene::Scene()
{
    this->_backColor = 0x4866ff;
	this->LoadContent();
}

Scene::~Scene()
{
}

void Scene::Update(float dt)
{
}

void Scene::LoadContent()
{
}

void Scene::Draw()
{
}

void Scene::Render()
{
    auto device = GameGlobal::GetCurrentDevice();

	if (SUCCEEDED(device->BeginScene()))
	{
		//begin render
		device->Clear(0, NULL, D3DCLEAR_TARGET, this->_backColor, 0.0f, 0);
		device->BeginScene();

		//draw
		GameGlobal::GetCurrentSpriteHandler()->Begin(D3DXSPRITE_ALPHABLEND);
		this->Draw();
		GameGlobal::GetCurrentSpriteHandler()->End();

		//end render
		device->EndScene();
	}

    device->Present(NULL, NULL, NULL, NULL);
}

void Scene::OnKeyDown(int keyCode)
{
}

void Scene::OnKeyUp(int keyCode)
{
}

void Scene::OnMouseDown(float x, float y)
{
}

D3DCOLOR Scene::GetBackcolor()
{
    return this->_backColor;
}