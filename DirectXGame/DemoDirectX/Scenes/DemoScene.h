#pragma once

#include <math.h>
#include <vector>
#include <d3dx9.h>
#include <d3d9.h>

#include "../GameComponents/Scene.h"
#include "../GameComponents/Sprite.h"
#include "../GameComponents/Animation.h"
#include "../GameComponents/GameMap.h"
#include "../GameComponents/Viewport.h"
#include "../GameComponents/GameDebugDraw.h"
#include "../GameObjects/Player/Player.h"

class DemoScene : public Scene
{
public:
	DemoScene();

	void Update(float dt);
	void LoadContent();
	void Draw();

	void OnKeyDown(int keyCode);
	void OnKeyUp(int keyCode);
	void OnMouseDown(float x, float y);

protected:
	void checkCollision(float dt);
	void DrawQuadtree(QuadTree* quadtree);
	void DrawCollidable();

	GameMap* _pGameMap;
	Viewport* _pViewport;
	Player* _player;

	map<int, bool> _keys;
	vector<Entity*> _collideAble;

	GameDebugDraw* mDebugDraw;
};

