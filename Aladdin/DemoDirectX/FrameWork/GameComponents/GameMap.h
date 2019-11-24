#ifndef __GAME_MAP__
#define __GAME_MAP__

#include <fstream>

#include "../GameComponents/Sprite.h"
#include "../MapReader/Tmx.h.in"
#include "Camera.h"
#include "GameGlobal.h"
#include "GameCollision.h"
#include "Grid.h"
#include "../GameObjects/Player/Player.h"
#include "../GameComponents/GameCollision.h"
#include "../GameObjects/Items.h"
#include "../GameObjects/Weapon/Apple.h"
#include "../GameObjects/Enemies/Guard_1st/Guard1.h"
#include "../GameObjects/Enemies/Guard_2nd/Guard2.h"
#include "../GameObjects/Enemies/Guard_3rd/Guard3.h"
#include "../GameObjects/Enemies/CivilianEnemy1st/CivilianEnemy1st.h"
#include "../GameObjects/Enemies/CivilianEnemy2nd/CivilianEnemy2nd.h"
#include "../GameObjects/Enemies/CivilianEnemy4th/CivilianEnemy4th.h"
#include "../GameObjects/Enemies/BatEnemy/BatEnemy.h"
#include "../GameObjects/MapObject/FloatingGround.h"
#include "../GameObjects/MapObject/Pendulum.h"
#include "../GameObjects/Jafar/Jafar.h"
#include "../GameObjects/MapObject/Camel.h"
#include "../GameObjects/MapObject/SpringBoard.h"
#include "../GameObjects/MapObject/Pelder.h"


class GameMap
{
public:
	~GameMap();
	static GameMap* GetInstance();

	void Initialize(char* filePath);

	static void Release();

	void LoadMapItems(char* filePath);
	void ReloadItems();

	void LoadMapObjects(char* filePath);
	void ReloadObjects();

	void InsertAppleAladdin(Apple* apple);
	void InsertStaticObject(Entity* entity);
	void InsertWeapon(Entity* weapon);

	void ClearList();
	void ClearEnemiesInRegion(RECT region);

	Player* mPlayer;

	Tmx::Map* GetMap();

	int GetWidth();
	int GetHeight();
	int GetTileWidth();
	int GetTileHeight();

	void SetCamera(Camera* camera);
	void Update(float dt);
	void Draw();

private:
	GameMap() = default;
	static GameMap* _instance;

	void LoadMap(char* filePath);
	bool isContain(RECT rect1, RECT rect2);

	Tmx::Map* mMap;
	Grid* mGrid;
	Camera* mCamera;

	char* mListItemsFile;
	char* mListObjectsFile;

	unordered_map<int, Sprite*> mTilesets;

	unordered_set<Entity*> mWeapons;
	unordered_set<Entity*> mEnemies;
	unordered_set<Apple*> mApples;
	unordered_set<Items*> mItems;

public:
	unordered_set<Items*>* GetItems()
	{
		return &mItems;
	}

	unordered_set<Entity*>* GetEnemies()
	{
		return &mEnemies;
	}

	unordered_set<Apple*>* GetApples()
	{
		return &mApples;
	}

	unordered_set<Entity*>* GetWeapons()
	{
		return &mWeapons;
	}
};

#endif

