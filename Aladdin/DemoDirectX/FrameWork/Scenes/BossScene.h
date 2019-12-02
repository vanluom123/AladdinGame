#pragma once

#include <math.h>
#include <vector>
#include <d3dx9.h>
#include <d3d9.h>

#include "../GameComponents/Sprite.h"
#include "../GameComponents/Scene.h"
#include "../GameComponents/Animation.h"
#include "../GameControllers/SpriteManager.h"
#include "../GameDefines/define.h"
#include "../GameObjects/Player/Player.h"
#include "../GameComponents/GameMap.h"
#include "../GameComponents/Camera.h"
#include "../GameComponents/GameDebugDraw.h"
#include "../GameObjects/Weapon/Apple.h"
#include "../GameObjects/PlayerInfo.h"
#include "../GameObjects/Weapon/Fire.h"
#include "../Scenes/EndScene.h"
class PlayerInfo;
class Scene_Final : public Scene
{
public:
	Scene_Final();
	Scene_Final(Player* player);
	void Update(float dt);
	void Draw();

	void OnKeyDown(int keyCode);
	void OnKeyUp(int keyCode);
	void OnMouseDown(float x, float y);
	void PlayMusic();
	void StopMusic();
	Scene::SceneName GetSceneName();
protected:
	LPD3DXSPRITE mSpriteHandler;

	float mTimeCounter;
	void checkCollision(float dt);
	
	void CheckCameraAndWorldMap();


	void DrawCollidable();

	Player *mPlayer;
	std::map<int, bool> keys;

	GameMap *mMap;
	Camera *mCamera;

	GameDebugDraw *mDebugDraw;
	unordered_set<Entity*> mCollidable;
	unordered_set<Entity*> *mListEnemyWeapons;
	unordered_set <Apple*> *mListApplesAladdin;
	unordered_set <Entity*> *mListEnemies;

	PlayerInfo *playerInfo;
	
	float timeResetApples;
	float timeCreateFire;
};

