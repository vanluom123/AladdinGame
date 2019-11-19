#include "GameMap.h"
#include "../GameObjects/MapObjects/BrickGold.h"
#include "../GameObjects/MapObjects/BrickNormal.h"

GameMap::GameMap(char* filePath)
{
	_pViewport = new Viewport(GameGlobal::GetWidth(), GameGlobal::GetHeight());
	_loadMap(filePath);
}

GameMap::~GameMap()
{
	delete _pTmxMap;

	for (auto brick : _listBrick)
		delete brick;

	if (!_listBrick.empty())
		_listBrick.clear();

	for (auto it = begin(_listTileset); it != end(_listTileset); ++it)
		delete it->second;

	if (!_listTileset.empty())
		_listTileset.clear();

	delete _pQuadTree;
}

void GameMap::_loadMap(char* filePath)
{
	_pTmxMap = new Tmx::Map();
	_pTmxMap->ParseFile(filePath);

	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = GetWidth();
	r.bottom = GetHeight();

	_pQuadTree = new QuadTree(1, r);

	for (size_t i = 0; i < _pTmxMap->GetNumTilesets(); i++)
	{
		const Tmx::Tileset* tileset = _pTmxMap->GetTileset(i);

		Sprite* sprite = new Sprite(tileset->GetImage()->GetSource().c_str());
		_listTileset.insert(pair<int, Sprite*>(i, sprite));
	}

#pragma region -BRICK AND COIN LAYER-

	for (size_t i = 0; i < GetMap()->GetNumTileLayers(); i++)
	{
		const Tmx::TileLayer* layer = _pTmxMap->GetTileLayer(i);

		if (layer->IsVisible())
			continue;

		if (layer->GetName() == "brick" || layer->GetName() == "coin")
		{
			for (size_t j = 0; j < _pTmxMap->GetNumTilesets(); j++)
			{
				const Tmx::Tileset* tileSet = _pTmxMap->GetTileset(j);

				int tileWidth = _pTmxMap->GetTileWidth();
				int tileHeight = _pTmxMap->GetTileHeight();

				int tileSetWidth = tileSet->GetImage()->GetWidth() / tileWidth;
				int tileSetHeight = tileSet->GetImage()->GetHeight() / tileHeight;

				for (size_t m = 0; m < layer->GetHeight(); m++)
				{
					for (size_t n = 0; n < layer->GetWidth(); n++)
					{
						if (layer->GetTileTilesetIndex(n, m) != -1)
						{
							int tileID = layer->GetTileId(n, m);

							int y = tileID / tileSetWidth;
							int x = tileID - y * tileSetWidth;

							GVector3 position((n * tileWidth) + (tileWidth / 2), (m * tileHeight) + (tileHeight / 2), 0);

							Brick* brick = nullptr;

							if (layer->GetName() == "coin")
							{
								brick = new BrickGold(position);
								brick->setTag(eID::BRICK_GOLD_NORMAL);
								_listBrick.push_back(brick);
							}
							else
							{
								brick = new BrickNormal(position);
								brick->setTag(eID::BRICK_NORMAL);
								_listBrick.push_back(brick);
							}

							if (brick)
								_pQuadTree->insertEntity(brick);
						}
					}
				}
			}
		}
	}

#pragma endregion

#pragma region -OBJECTGROUP, STATIC OBJECT-

	_insertObjectMap();

#pragma endregion
}

void GameMap::_insertObjectMap()
{
	for (size_t i = 0; i < _pTmxMap->GetNumObjectGroups(); i++)
	{
		const Tmx::ObjectGroup* objectGroup = _pTmxMap->GetObjectGroup(i);
		string name = objectGroup->GetName();
		
		if(name == "Wall")
		{
			for (size_t j = 0; j < objectGroup->GetNumObjects(); j++)
			{
				Tmx::Object* object = objectGroup->GetObjects().at(j);

				Entity* entity = new Entity();
				entity->setPosition(GVector3(object->GetX() + object->GetWidth() / 2, object->GetY() + object->GetHeight() / 2, 0));
				entity->SetWidth(object->GetWidth());
				entity->SetHeight(object->GetHeight());
				entity->setTag(WALL);
				_pQuadTree->insertEntity(entity);
			}
		}
	}
}

void GameMap::SetCamera(Viewport* camera)
{
	_pViewport = camera;
}

Tmx::Map* GameMap::GetMap()
{
	return  _pTmxMap;
}

RECT GameMap::GetWorldMapBound()
{
	RECT bound;
	bound.left = bound.top = 0;
	bound.right = _pTmxMap->GetWidth() * _pTmxMap->GetTileWidth();
	bound.bottom = _pTmxMap->GetHeight() * _pTmxMap->GetTileHeight();

	return bound;
}

int GameMap::GetWidth()
{
	return  _pTmxMap->GetWidth() * _pTmxMap->GetTileWidth();
}

int GameMap::GetHeight()
{
	return  _pTmxMap->GetHeight() * _pTmxMap->GetTileHeight();
}

int GameMap::GetTileWidth()
{
	return  _pTmxMap->GetTileWidth();
}

int GameMap::GetTileHeight()
{
	return  _pTmxMap->GetTileHeight();
}

bool GameMap::IsBoundLeft()
{
	return (_pViewport->GetBound().left == 0);
}

bool GameMap::IsBoundRight()
{
	return (_pViewport->GetBound().right == GetWidth());
}

bool GameMap::IsBoundTop()
{
	return (_pViewport->GetBound().top == 0);
}

bool GameMap::IsBoundBottom()
{
	return (_pViewport->GetBound().bottom == GetHeight());
}

void GameMap::Update(float dt)
{
	for each (auto item in _listBrick)
		item->Update(dt);
}

void GameMap::Draw()
{

#pragma region DRAW TILESET
	for (size_t i = 0; i < _pTmxMap->GetNumTileLayers(); i++)
	{
		const Tmx::TileLayer* layer = _pTmxMap->GetTileLayer(i);

		if (!layer->IsVisible())
		{
			continue;
		}

		for (size_t j = 0; j < _pTmxMap->GetNumTilesets(); j++)
		{
			const Tmx::Tileset* tileSet = _pTmxMap->GetTileset(j);

			RECT srcRect = RECT();

			int tileWidth = _pTmxMap->GetTileWidth();
			int tileHeight = _pTmxMap->GetTileHeight();

			int tileSetWidth = tileSet->GetImage()->GetWidth() / tileWidth;
			int tileSetHeight = tileSet->GetImage()->GetHeight() / tileHeight;

			for (size_t row = 0; row < layer->GetHeight(); row++)
			{
				for (size_t col = 0; col < layer->GetWidth(); col++)
				{
					if (layer->GetTileTilesetIndex(col, row) != -1)
					{
						int tileID = layer->GetTileId(col, row);

						int y = tileID / tileSetWidth;
						int x = tileID - y * tileSetWidth;

						srcRect.top = y * tileHeight;
						srcRect.bottom = srcRect.top + tileHeight;
						srcRect.left = x * tileWidth;
						srcRect.right = srcRect.left + tileWidth;

						Sprite* sprite = _listTileset.at(j);

						GVector3 position((col * tileWidth) + (tileWidth / 2), (row * tileHeight) + (tileHeight / 2), 0);

						if (_pViewport != NULL)
						{
							RECT r;
							r.left = position.x - tileWidth / 2;
							r.top = position.y - tileHeight / 2;
							r.right = r.left + tileWidth;
							r.bottom = r.top + tileHeight;

							if(!GameCollision::isCollide(_pViewport->GetBound(), r))
								continue;;
						}

						sprite->setWidth(tileWidth);
						sprite->setHeight(tileHeight);

						sprite->render(position, srcRect, GVector2(), _pViewport->getTranslate());
					}
				}
			}
		}
	}
#pragma endregion

#pragma region DRAW BRICK

	for each (auto listBricks in _listBrick)
		listBricks->Draw(_pViewport->getTranslate());

#pragma endregion
}

map<int, Sprite*> GameMap::getListTileSet()
{
	return _listTileset;
}

vector<Brick*> GameMap::GetListBrick()
{
	return _listBrick;
}

QuadTree* GameMap::GetQuadTree()
{
	return _pQuadTree;
}
