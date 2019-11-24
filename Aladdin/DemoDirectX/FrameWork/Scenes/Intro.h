#pragma once

#include <math.h>
#include "../GameComponents/Sprite.h"
#include "../GameComponents/Scene.h"
#include "../GameComponents/Animation.h"
#include "../GameControllers/SpriteManager.h"
#include "../GameDefines/define.h"
#include "../GameObjects/Player/Player.h"
#include "../GameComponents/Camera.h"
#include "../GameComponents/GameDebugDraw.h"
#include "../GameObjects/Weapon/Apple.h"
#include "../GameObjects/PlayerInfo.h"
#include "../GameObjects/Weapon/Fire.h"
#include "../../FrameWork/GameControllers/SceneManager.h"
#include "Scene1.h"
#include "BeginScene.h"

class Intro : public Scene
{
public:
	Intro();
	Intro(int numIntro);
	void Update(float dt);
	void Draw();

	void OnKeyDown(int keyCode);
	void OnKeyUp(int keyCode);

	Scene::SceneName GetSceneName();

protected:
	LP_SPRITE mSpriteHandler;

	std::map<int, bool> keys;

	Animation* animation_1;
	int currentIntro;
	int nextIntro;

};

