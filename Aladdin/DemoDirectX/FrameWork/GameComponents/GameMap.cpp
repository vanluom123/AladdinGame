#include "GameMap.h"
#include "../GameObjects/MapObject/Trap.h"
#include "../GameObjects/MapObject/BrickDynamic.h"
#include "../GameObjects/Enemies/Sketelon/Sketelon.h"
#include "../GameObjects/Enemies/Sketelon/Mummies.h"

GameMap::GameMap(char* filePath)
{
	mListApples = new std::vector<Apple*>;
	mListEnemies = new std::vector<Entity*>;
	mListWeapons = new std::vector<Entity*>;
	LoadMap(filePath);
}

GameMap::~GameMap()
{
	delete mMap;
}

Grid* GameMap::GetGrid()
{
	return this->mGrid;
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

	mGrid = new Grid();

	for (size_t i = 0; i < mMap->GetNumTilesets(); i++)
	{
		const Tmx::Tileset* tileset = mMap->GetTileset(i);

		Sprite* sprite = new Sprite(tileset->GetImage()->GetSource().c_str(), RECT(), NULL, NULL, D3DCOLOR_XRGB(0x00, 0xff, 0xff));

		mListTileset.insert(std::pair<int, Sprite*>(i, sprite));
	}

#pragma region -OBJECTGROUP, STATIC OBJECT-

	for (size_t i = 0; i < mMap->GetNumObjectGroups(); i++)
	{
		const Tmx::ObjectGroup* objectGroup = mMap->GetObjectGroup(i);

		if (!objectGroup->IsVisible())
			continue;//Nawty Node

		for (size_t j = 0; j < objectGroup->GetNumObjects(); j++)
		{
			//lay object group chu khong phai layer
			//object group se chua nhung body
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

			fscanf(file, "%s %f %f", &name, &x, &y);

			GVector2 position(x, y);
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

			mListItems.push_back(tmp);
			mGrid->InsertEntity(tmp);
		}

		fclose(file);
	}
}

void GameMap::ReloadItems()
{
	for (int i = 0; i < mListItems.size(); i++)
	{
		mGrid->RemoveEntiy(mListItems.at(i));
		delete mListItems.at(i);
	}
	mListItems.clear();
	this->LoadMapItems(mListItemsFile);
}

void GameMap::ReloadObjects()
{
	for (int i = 0; i < mListEnemies->size(); i++)
	{
		delete mListEnemies->at(i);
	}
	mListEnemies->clear();
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
			char entityType[100];
			char info[1000];
			fgets(info, 100, file);
			char nameType[100];
			char status[100];
			GVector2 position;
			Entity* tmp = nullptr;

			fscanf(file, "%s %s %f %f %s", &entityType, &nameType, &position.x, &position.y, &status);

			Entity::EntityTypes types = static_cast<Entity::EntityTypes>(atoi(entityType));

			switch (types)
			{
			case Entity::TYPE_PENDULUM:
			{
				auto pen = new Pendulum(position);
				pen->SetStatus(status);
				tmp = pen;
			}
			break;

			case Entity::TYPE_TRAP:
			{
				auto trap = new Trap(position);
				trap->SetStatus(status);
				tmp = trap;
			}
			break;

			case Entity::TYPE_BRICK:
			{
				auto brick = new BrickDynamic(position);
				brick->SetStatus(status);
				tmp = brick;
				mGrid->InsertEntity(tmp);
			}
			break;

			case Entity::TYPE_SKETELON:
				tmp = new Sketelon(position);
				break;

			case Entity::TYPE_MUMMIES:
				tmp = new Mummies(position);
				break;

			default:
				tmp = nullptr;
				break;
			}

			if (tmp != nullptr)
				mListEnemies->push_back(tmp);
		}

		fclose(file);
	}
}

bool GameMap::isContain(RECT rect1, RECT rect2)
{
	auto left = rect1.left - rect2.right;
	auto top = rect1.top - rect2.bottom;
	auto right = rect1.right - rect2.left;
	auto bottom = rect1.bottom - rect2.top;

	return !(left > 0 || right < 0 || top > 0 || bottom < 0);
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
	for (size_t i = 0; i < mListItems.size(); i++)
	{
		RECT objRECT = mListItems.at(i)->GetBound();

		//neu nam ngoai camera thi delete
		if (isContain(objRECT, mCamera->GetBound()) == false)
		{
			mListItems.at(i)->SetDraw(false);
			continue;
		}
		else
			mListItems.at(i)->SetDraw(true);

		if (mListItems.at(i)->IsDeleted() == true)
		{
			mGrid->RemoveEntiy(mListItems.at(i));
			mListItems.erase(mListItems.begin() + i);
		}
		else
			mListItems.at(i)->Update(dt);
	}
#pragma endregion

#pragma region UPDATE_APPLES
	for (size_t i = 0; i < mListApples->size(); i++)
	{
		// Nawty Note
		{
			RECT r = mListApples->at(i)->GetBound();

			// neu nam ngoai camera thi delete
			if (isContain(r, mCamera->GetBound()) == false)
			{
			}
		}

		if (mListApples->at(i)->IsDeleted() == true)
			mListApples->erase(mListApples->begin() + i);
		else
			mListApples->at(i)->Update(dt);
	}
#pragma endregion

#pragma region UPDATE_ENEMIES
	for (size_t i = 0; i < mListEnemies->size(); i++)
	{
		{
			RECT r = mListEnemies->at(i)->GetBound();

			if (isContain(r, mCamera->GetBound()) == false)
			{
				mListEnemies->at(i)->SetDraw(false);
				if (mListEnemies->at(i)->Tag == Entity::FloatingGround || mListEnemies->at(i)->Tag == Entity::Jafar)
					mListEnemies->at(i)->Update(dt);

				continue;
			}
			else
				mListEnemies->at(i)->SetDraw(true);
		}

		if (mListEnemies->at(i)->IsDeleted() == true)
			mListEnemies->erase(mListEnemies->begin() + i);
		else
			mListEnemies->at(i)->Update(dt);
	}

#pragma endregion UPDATE_ENEMIES

#pragma region UPDATE_ENEMY_WEAPONS
	for (size_t i = 0; i < mListWeapons->size(); i++)
	{
		RECT r = mListWeapons->at(i)->GetBound();

		//neu nam ngoai camera thi delete
		if (isContain(r, mCamera->GetBound()) == false)
			mListWeapons->at(i)->SetDraw(false);
		else
			mListWeapons->at(i)->SetDraw(true);

		if (mListWeapons->at(i)->IsDeleted() == true)
			mListWeapons->erase(mListWeapons->begin() + i);
		else
			mListWeapons->at(i)->Update(dt);
	}
#pragma endregion UPDATE_ENEMY_WEAPONS
}

void GameMap::Draw()
{
	auto trans_x = GameGlobal::GetWidth() / 2 - mCamera->GetPosition().x;
	auto trans_y = GameGlobal::GetHeight() / 2 - mCamera->GetPosition().y;
	GVector2 trans = GVector2(trans_x, trans_y);

#pragma region Draw_MAP
	for (size_t i = 0; i < mMap->GetNumTileLayers(); i++)
	{
		const Tmx::TileLayer* layer = mMap->GetTileLayer(i);

		if (!layer->IsVisible())
			continue;

		if (i == 1)//Nawty Note i==1
		{
			for (size_t i = 0; i < mListItems.size(); i++)
			{
				if (mListItems.at(i)->IsDraw() == true)
					mListItems.at(i)->Draw(trans);
			}
			for (size_t i = 0; i < mListEnemies->size(); i++)
			{
				if (mListEnemies->at(i)->IsDraw() == true)
					mListEnemies->at(i)->Draw(trans);
			}

			for (size_t i = 0; i < mListApples->size(); i++)
			{
				if (mListApples->at(i)->IsDraw() == true)
					mListApples->at(i)->Draw(mListApples->at(i)->GetPosition(), RECT(), GVector2(), trans);
			}
			for (size_t i = 0; i < mListWeapons->size(); i++)
			{
				if (mListWeapons->at(i)->IsDraw() == true)
					mListWeapons->at(i)->Draw(trans);
			}
			mPlayer->Draw();
		}

		RECT srcRect;

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

					Sprite* sprite = mListTileset[layer->GetTileTilesetIndex(n, m)];

					//tile index
					int tileID = layer->GetTileId(n, m);

					int y = tileID / tileSetWidth;
					int x = tileID - y * tileSetWidth;

					srcRect.top = y * tileHeight;
					srcRect.bottom = srcRect.top + tileHeight;
					srcRect.left = x * tileWidth;
					srcRect.right = srcRect.left + tileWidth;

					//tru tilewidth/2 va tileheight/2 vi Sprite ve o vi tri giua hinh anh cho nen doi hinh de cho
					//dung toa do (0,0) cua the gioi thuc la (0,0) neu khong thi se la (-tilewidth/2, -tileheigth/2);
					D3DXVECTOR3 position(n * tileWidth + tileWidth / 2, m * tileHeight + tileHeight / 2, 0);

					if (mCamera != NULL)
					{
						RECT rectObject;
						rectObject.left = position.x - tileWidth / 2;
						rectObject.top = position.y - tileHeight / 2;
						rectObject.right = rectObject.left + tileWidth;
						rectObject.bottom = rectObject.top + tileHeight;

						//neu nam ngoai camera thi khong Draw
						if (isContain(rectObject, mCamera->GetBound()) == false)
							continue;
					}

					sprite->SetWidth(tileWidth);
					sprite->SetHeight(tileHeight);
					sprite->Draw(position, srcRect, GVector2(), trans);
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
	mListApples->push_back(apple);
}

void GameMap::InsertStaticObject(Entity* entity)
{

	mGrid->InsertEntity(entity);
}
void GameMap::InsertWeapon(Entity* weapon)
{
	mListWeapons->push_back(weapon);
}

std::vector<Apple*>* GameMap::GetListApples()
{
	return this->mListApples;
}

std::vector<Entity*>* GameMap::GetListWeapons()
{
	return this->mListWeapons;
}
std::vector<Entity*>* GameMap::GetListEnemies()
{
	return this->mListEnemies;
}

void GameMap::ClearList()
{
}

void GameMap::ClearEnemiesInRegion(RECT region)
{
	for (size_t i = 0; i < mListEnemies->size(); i++)
	{
		if (GameCollision::AABBCheck(mListEnemies->at(i)->GetBound(), region) == true)
			mListEnemies->at(i)->SetDelete(true);
	}
}