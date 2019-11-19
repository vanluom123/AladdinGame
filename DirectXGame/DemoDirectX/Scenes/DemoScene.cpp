#include "DemoScene.h"
#include "../GameDefines/GameDefine.h"
#include "../GameComponents/GameLog.h"
#include "../GameComponents/Sound.h"
#include "../GameControllers/SceneManager.h"
#include "../Transition/TransitionCircleScan.h"
#include "TestScene.h"


DemoScene::DemoScene()
{
	LoadContent();
}

void DemoScene::LoadContent()
{
	//set mau backcolor cho scene o day la mau xanh
	_backColor = 0x54acd2;

	Sound::getInstance()->loadSound("Resources/man1.wav", "man1");
	Sound::getInstance()->play("man1", true, 0);

	mDebugDraw = new GameDebugDraw();

	_pGameMap = new GameMap("Resources/marioworld1-1.tmx");

	_pViewport = new Viewport(GameGlobal::GetWidth(), GameGlobal::GetHeight());
	_pViewport->SetPosition(GameGlobal::GetWidth() / 2, _pGameMap->GetHeight() - _pViewport->GetHeight());

	_pGameMap->SetCamera(_pViewport);

	_player = new Player();
	_player->setPosition(GVector3(GameGlobal::GetWidth() / 2, GameGlobal::GetHeight() / 2, 0));
	_player->setViewport = _pViewport;

	_pViewport->checkViewportWithMapWorld(_player, _pGameMap);
}

void DemoScene::Update(float dt)
{
	this->checkCollision(dt);
	_pGameMap->Update(dt);
	_player->keyboardEventHandler(_keys);
	_player->update(dt);
	_pViewport->checkViewportWithMapWorld(_player, _pGameMap);
}

void DemoScene::Draw()
{
	_pGameMap->Draw();

	_player->draw();

	DrawQuadtree(_pGameMap->GetQuadTree());

	DrawCollidable();
}

void DemoScene::OnKeyDown(int keyCode)
{
	_keys[keyCode] = true;

	_player->onKeyPressed(keyCode);
}

void DemoScene::OnKeyUp(int keyCode)
{
	_keys[keyCode] = false;

	_player->onKeyUp(keyCode);
}

void DemoScene::OnMouseDown(float x, float y)
{
	SceneManager::GetInstance()->ReplaceScene(new TestScene(), new TransitionCircleScan());
}

void DemoScene::checkCollision(float dt)
{
	_collideAble.clear();

	vector<Entity*> listCollision;
	_pGameMap->GetQuadTree()->getEntitiesCollideAble(listCollision, _player);

	for (auto child : listCollision)
	{
		_collideAble.push_back(child);
	}

	Entity::eSide side;

	for each (auto objCollision in listCollision)
	{
		GVector2 distance = GameCollision::getDistance(_player, objCollision, dt);
		RECT broadphase = GameCollision::getBroadphase(_player->GetBounding(), distance);

		if (GameCollision::isCollide(broadphase, objCollision->GetBounding()) == true)
		{
			float collisionTime = GameCollision::sweptAABB(_player->GetBounding(), objCollision->GetBounding(), distance, side);
			if (collisionTime < 1.0f)
			{
				_player->CheckCollision(objCollision, side, collisionTime);
			}
		}
	}
}

void DemoScene::DrawQuadtree(QuadTree* quadtree)
{
	if (quadtree->getNodes()->empty() == false)
	{
		for (auto node : (*quadtree->getNodes()))
			this->DrawQuadtree(node);
	}

	mDebugDraw->DrawRect(quadtree->getBounding(), _pViewport);

	if (quadtree->getNodes()->empty() == false)
	{
		for (auto node : (*quadtree->getNodes()))
			mDebugDraw->DrawRect(node->getBounding(), _pViewport);
	}
}

void DemoScene::DrawCollidable()
{
	for (auto child : _collideAble)
	{
		mDebugDraw->DrawRect(child->GetBounding(), _pViewport);
	}
}
