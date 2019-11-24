#ifndef __SCENE1_H__
#define __SCENE1_H__

#include <math.h>
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
	~Scene1();

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
	void CheckCollisionEnemies(float dt);
	void EnemiesWithStaticObject(float dt);
	void CheckCameraAndWorldMap();

	LP_SPRITE mSpriteHandler;

	Camera* mCamera;
	Player* mPlayer;
	PlayerInfo* playerInfo;

	map<int, bool> keys;
	unordered_set<Entity*> mCollidable;

	float mTimeCounter;
};

#endif // __SCENE1_H__
