#include "GameMap.h"

#pragma warning(disable : 26812)

GameMap* GameMap::_instance = nullptr;

GameMap::~GameMap()
{
	delete mMap;
	Grid::Release();

	for (auto it : mEnemies)
		delete it;
	mEnemies.clear();

	for (auto it : mItems)
		delete it;
	mItems.clear();

	for (auto it : mApples)
		delete it;
	mApples.clear();

	for (auto it : mWeapons)
		delete it;
	mWeapons.clear();
}

GameMap* GameMap::GetInstance()
{
	if (_instance == nullptr)
		_instance = new GameMap();
	return _instance;
}

void GameMap::Initialize(char* filePath)
{
	mGrid = Grid::GetInstance();
	this->LoadMap(filePath);
}

void GameMap::Release()
{
	delete _instance;
	_instance = nullptr;
}

void GameMap::LoadMap(char* filePath)
{
	mMap = new Tmx::Map();
	mMap->ParseFile(filePath);

	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = this->GetWidth();
	r.bottom = this->GetHeight();

	for (size_t i = 0; i < mMap->GetNumTilesets(); i++)
	{
		const Tmx::Tileset* tileset = mMap->GetTileset(i);
		Sprite* sprite = new Sprite(tileset->GetImage()->GetSource().c_str(), RECT(), NULL, NULL, D3DCOLOR_XRGB(0x00, 0xff, 0xff));
		mTilesets.insert(std::pair<int, Sprite*>(i, sprite));
	}

#pragma region -OBJECTGROUP, STATIC OBJECT-

	for (size_t i = 0; i < mMap->GetNumObjectGroups(); i++)
	{
		const Tmx::ObjectGroup* objectGroup = mMap->GetObjectGroup(i);
		
		// Visible object
		if (!objectGroup->IsVisible())
			continue;

		for (size_t j = 0; j < objectGroup->GetNumObjects(); j++)
		{
			Tmx::Object* object = objectGroup->GetObjects().at(j);

			Entity* entity = new Entity();
			int pos_x = object->GetX() + object->GetWidth() / 2;
			int pos_y = object->GetY() + object->GetHeight() / 2;
			entity->SetPosition(pos_x, pos_y);
			entity->SetWidth(object->GetWidth());
			entity->SetHeight(object->GetHeight());

			if (objectGroup->GetName() == "Ground")
			{
				entity->Tag = Entity::Ground;
			}
			else if (objectGroup->GetName() == "VerticalRope")
			{
				entity->Tag = Entity::VerticalRope;
			}

			mGrid->InsertEntity(entity);
		}

	}
#pragma endregion

}

void GameMap::LoadMapItems(char* filePath)
{
	this->mListItemsFile = filePath;
	FILE* file;
	file = fopen(filePath, "r");

	if (file)
	{
		while (!feof(file))
		{
			char name[100];
			char info[1000];
			fgets(info, 100, file);
			float x, y;
			(void)fscanf(file, "%s %f %f", &name, &x, &y);
			D3DXVECTOR2 position = D3DXVECTOR2(x, y);
			Items* tmp;

			char chr = name[strlen(name) - 1];
			switch (chr)
			{
			case 'a':
				tmp = new Items(position, Items::ItemTypes::Apple);
				break;
			case '1':
				tmp = new Items(position, Items::ItemTypes::OneUP);
				break;
			case 's':
				tmp = new Items(position, Items::ItemTypes::Store);
				break;
			case 'm':
				tmp = new Items(position, Items::ItemTypes::MagicLamp);
				break;
			case 'g':
				tmp = new Items(position, Items::ItemTypes::GenieBonusLevel);
				break;
			case 'e':
				tmp = new Items(position, Items::ItemTypes::ExtraHealth);
				break;
			case 'r':
				tmp = new Items(position, Items::ItemTypes::RestartPoint);
				break;
			case 'i':
				tmp = new Items(position, Items::ItemTypes::Information);
				break;
			case 'A':
				tmp = new Items(position, Items::ItemTypes::AbuBonusLevel);
				break;
			case 'S':
				tmp = new Items(position, Items::ItemTypes::SpendThese);
				break;

			default:
				tmp = new Items(position, Items::ItemTypes::OneUP);
				break;
			}

			mItems.insert(tmp);
			mGrid->InsertEntity(tmp);
		}

		fclose(file);
	}
}

void GameMap::ReloadItems()
{
	for (auto it : mItems)
	{
		mGrid->RemoveEntiy(it);
		delete it;
	}
	mItems.clear();
	this->LoadMapItems(mListItemsFile);
}

void GameMap::ReloadObjects()
{
	for (auto it : mEnemies)
	{
		mGrid->RemoveEntiy(it);
		delete it;
	}
	mEnemies.clear();
	this->LoadMapObjects(mListObjectsFile);
}

void GameMap::LoadMapObjects(char* filePath)
{
	this->mListObjectsFile = filePath;
	FILE* file;
	file = fopen(filePath, "r");

	if (file)
	{
		while (!feof(file))
		{
			char name[100];
			char info[1000];
			fgets(info, 100, file);
			char type[100];
			float x, y;
			(void)fscanf(file, "%s %s %f %f", &name, &type, &x, &y);
			D3DXVECTOR2 position = D3DXVECTOR2(x, y);
			Entity* tmp = nullptr;
			Items* temp;
			char chr = name[strlen(name) - 1];
			switch (chr)
			{
			case '1':
				tmp = new Guard1(position, this->mPlayer);
				break;
			case '2':
				tmp = new Guard2(position, this->mPlayer);
				break;
			case '3':
			{
				Guard3* temp = new Guard3(position, this->mPlayer);
				temp->SetGameMap(this);
				tmp = temp;
			}
			break;
			case '4':
			{
				CivilianEnemy1st* temp = new CivilianEnemy1st(position, this->mPlayer);
				temp->SetGameMap(this);
				tmp = temp;
			}
			break;
			case '5':
			{
				CivilianEnemy2nd* temp = new CivilianEnemy2nd(position, this->mPlayer);
				temp->SetGameMap(this);
				tmp = temp;
			}
			break;
			case '7':
			{
				CivilianEnemy4th* temp = new CivilianEnemy4th(position, this->mPlayer);
				tmp = temp;
			}
			break;
			case 'b':
			{
				BatEnemy* temp = new BatEnemy(position, this->mPlayer);
				temp->SetGameMap(this);
				tmp = temp;
			}
			break;
			case 'p':
				tmp = new class Pendulum(position, this);
				break;
			case 'f':
				tmp = new FloatingGround(D3DXVECTOR3(position.x, position.y, 0));
				break;
			case 'j':
				tmp = new class Jafar(position, mPlayer);
				break;
			case 'a':
				tmp = new class Camel(position, this);
				break;
			case 's':
				tmp = new class SpringBoard(position);
				break;
			case 'S':
				tmp = new class Pelder(position);
				break;
			default:
				tmp = nullptr;
				break;
			}
			if (tmp != nullptr)
			{
				mEnemies.insert(tmp);
			}
		}

		fclose(file);
	}
}

bool GameMap::isContain(RECT rect1, RECT rect2)
{
	if (rect1.left > rect2.right || rect1.right < rect2.left || rect1.top > rect2.bottom || rect1.bottom < rect2.top)
	{
		return false;
	}

	return true;
}

Tmx::Map* GameMap::GetMap()
{
	return mMap;
}

int GameMap::GetWidth()
{
	return mMap->GetWidth() * mMap->GetTileWidth();

}

int GameMap::GetHeight()
{
	//return mMap->GetHeight();
	return mMap->GetHeight() * mMap->GetTileHeight();
}

int GameMap::GetTileWidth()
{
	return mMap->GetTileWidth();
}

int GameMap::GetTileHeight()
{
	return mMap->GetTileHeight();
}

void GameMap::Update(float dt)
{

#pragma region UPDATE_ITEMS

	for (auto it : mItems)
	{
		RECT obj = it->GetBound();
		if (isContain(obj, mCamera->GetBound()) == false)
		{
			it->SetDraw(false);
			continue;
		}
		else
			it->SetDraw(true);

		if (it->IsDeleted() == true)
		{
			mGrid->RemoveEntiy(it);
			mItems.erase(it);
		}
		else
			it->Update(dt);
	}

#pragma endregion

#pragma region UPDATE_APPLES

	for (auto it : mApples)
	{
		if (it->IsDeleted() == true)
			mApples.erase(it);
		else
			it->Update(dt);
	}
#pragma endregion

#pragma region UPDATE_ENEMIES

	for (auto it : mEnemies)
	{
		RECT obj = it->GetBound();
		if (isContain(obj, mCamera->GetBound()) == false)
		{
			it->SetDraw(false);
			if (it->Tag == Entity::FloatingGround || it->Tag == Entity::Jafar)
				it->Update(dt);
			continue;
		}
		else
			it->SetDraw(true);

		if (it->IsDeleted() == true)
			mEnemies.erase(it);
		else
			it->Update(dt);
	}
#pragma endregion UPDATE_ENEMIES

#pragma region UPDATE_ENEMY_WEAPONS

	for (auto it : mWeapons)
	{
		RECT obj = it->GetBound();
		if (isContain(obj, mCamera->GetBound()) == false)
			it->SetDraw(false);
		else
			it->SetDraw(true);

		if (it->IsDeleted() == true)
			mWeapons.erase(it);
		else
			it->Update(dt);
	}
#pragma endregion UPDATE_ENEMY_WEAPONS
}

void GameMap::Draw()
{
	D3DXVECTOR2 trans = D3DXVECTOR2(GameGlobal::GetWidth() / 2 - mCamera->GetPosition().x,
		GameGlobal::GetHeight() / 2 - mCamera->GetPosition().y);

#pragma region Draw_MAP
	for (size_t i = 0; i < mMap->GetNumTileLayers(); i++)
	{
		const Tmx::TileLayer* layer = mMap->GetTileLayer(i);

		if (!layer->IsVisible())
		{
			continue;
		}
		if (i == 1)
		{
			for (auto it : mItems)
			{
				if (it->IsDraw() == true)
					it->Draw(trans);
			}
			for (auto it : mEnemies)
			{
				if (it->IsDraw() == true)
					it->Draw(trans);
			}

			for (auto it : mApples)
			{
				if (it->IsDraw() == true)
					it->Draw(it->GetPosition(), RECT(), D3DXVECTOR2(), trans);
			}
			for (auto it : mWeapons)
			{
				if (it->IsDraw() == true)
					it->Draw(trans);
			}

			mPlayer->Draw();
		}

		RECT sourceRECT;

		int tileWidth = mMap->GetTileWidth();
		int tileHeight = mMap->GetTileHeight();

		for (size_t m = 0; m < layer->GetHeight(); m++)
		{
			for (size_t n = 0; n < layer->GetWidth(); n++)
			{
				int tilesetIndex = layer->GetTileTilesetIndex(n, m);

				if (tilesetIndex != -1)
				{
					const Tmx::Tileset* tileSet = mMap->GetTileset(tilesetIndex);

					int tileSetWidth = tileSet->GetImage()->GetWidth() / tileWidth;
					int tileSetHeight = tileSet->GetImage()->GetHeight() / tileHeight;

					Sprite* sprite = mTilesets[layer->GetTileTilesetIndex(n, m)];

					//tile index
					int tileID = layer->GetTileId(n, m);

					int y = tileID / tileSetWidth;
					int x = tileID - y * tileSetWidth;

					sourceRECT.top = y * tileHeight;
					sourceRECT.bottom = sourceRECT.top + tileHeight;
					sourceRECT.left = x * tileWidth;
					sourceRECT.right = sourceRECT.left + tileWidth;

					//tru tilewidth/2 va tileheight/2 vi Sprite ve o vi tri giua hinh anh cho nen doi hinh de cho
					//dung toa do (0,0) cua the gioi thuc la (0,0) neu khong thi se la (-tilewidth/2, -tileheigth/2);
					D3DXVECTOR3 position(n * tileWidth + tileWidth / 2, m * tileHeight + tileHeight / 2, 0);

					if (mCamera != NULL)
					{
						RECT objRECT;
						objRECT.left = position.x - tileWidth / 2;
						objRECT.top = position.y - tileHeight / 2;
						objRECT.right = objRECT.left + tileWidth;
						objRECT.bottom = objRECT.top + tileHeight;

						//neu nam ngoai camera thi khong Draw
						if (isContain(objRECT, mCamera->GetBound()) == false)
							continue;
					}

					sprite->SetWidth(tileWidth);
					sprite->SetHeight(tileHeight);
					sprite->Draw(position, sourceRECT, D3DXVECTOR2(), trans);
				}
			}
		}
	}
#pragma endregion

}

void GameMap::SetCamera(Camera* camera)
{
	this->mCamera = camera;
}

void GameMap::InsertAppleAladdin(Apple* apple)
{
	mApples.insert(apple);
}

void GameMap::InsertStaticObject(Entity* entity)
{
	mGrid->InsertEntity(entity);
}

void GameMap::InsertWeapon(Entity* weapon)
{
	mWeapons.insert(weapon);
}

void GameMap::ClearList()
{}

void GameMap::ClearEnemiesInRegion(RECT region)
{
	for (auto it : mEnemies)
	{
		if (GameCollision::isCollide(it->GetBound(), region) == true)
			it->SetDelete(true);
	}
}