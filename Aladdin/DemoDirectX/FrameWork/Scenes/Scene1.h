#pragma once

#include <math.h>
#include <unordered_set>
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
#include "BossScene.h"
#include "../GameControllers/SceneManager.h"
#include "Intro.h"
#include "EndScene.h"
class Scene1 : public Scene
{
public:
	Scene1();
	void Update(float dt);
	void LoadContent();
	void Draw();

	void OnKeyDown(int keyCode);
	void OnKeyUp(int keyCode);
	void OnMouseDown(float x, float y);
	void PlayMusic();
	void StopMusic();
	Scene::SceneName GetSceneName();

protected:
	void checkCollision(float dt);
	void enemyWeaponCollideWith(unordered_set<Entity*>& objectsCollision);
	void appleCollideWith(unordered_set<Entity*>& objectsCollision);
	void playerCollideWith(unordered_set<Entity*>& objectsCollision, float dt);
	void CheckCameraAndWorldMap();

	LP_SPRITE mSpriteHandler;
	float mTimeCounter;

	Player* mPlayer;
	map<int, bool> keys;
	GameMap* mMap;
	Camera* mCamera;

	unordered_set<Entity*> mCollidable;
	unordered_set<Entity*>* mListEnemyWeapons;
	unordered_set<Apple*>* mListApplesAladdin;
	unordered_set<Entity*>* mListEnemies;

	PlayerInfo* playerInfo;
};

