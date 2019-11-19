#ifndef __GAME_MAP__
#define __GAME_MAP__

#include "../GameComponents/Sprite.h"
#include "../MapReader/Tmx.h.in"
#include "Viewport.h"
#include "GameGlobal.h"
#include "GameCollision.h"
#include "QuadTree.h"
#include "../GameObjects/MapObjects/Brick.h"

class GameMap
{
public:
	GameMap(char* filePath);
	void SetCamera(Viewport* camera);
	void Update(float dt);
	void Draw();
	Tmx::Map* GetMap();
	RECT GetWorldMapBound();
	int GetWidth();
	int GetHeight();
	int GetTileWidth();
	int GetTileHeight();
	map<int, Sprite*> getListTileSet();

	bool IsBoundLeft(); //kiem tra luc nay Camera o vi bien ben trai so voi WorldMap
	bool IsBoundRight(); // kiem tra xem co o vi tri bien ben phai worldmap khong
	bool IsBoundTop(); // kiem tra xem co o vi tri bien ben trai worldmap khong
	bool IsBoundBottom(); // kiem tra xem co o vi tri bien ben phai worldmap khong
	~GameMap();

	vector<Brick*> GetListBrick();

	QuadTree* GetQuadTree();

private:
	void _loadMap(char* filePath);

	void _insertObjectMap();

	Tmx::Map* _pTmxMap;

	// List tile set
	map<int, Sprite*> _listTileset;

	// List bricks
	vector<Brick*> _listBrick;

	// Camera
	Viewport* _pViewport;

	// Quad tree
	QuadTree* _pQuadTree;

	Sprite* _spriteBrick, * _spriteGoldBrick;
};

#endif

