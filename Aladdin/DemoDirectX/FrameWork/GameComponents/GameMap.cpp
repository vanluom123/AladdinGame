#include "GameMap.h"
#include "../GameObjects/MapObject/Trap.h"
#include "../GameObjects/MapObject/BrickDynamic.h"
#include "../GameObjects/Enemies/Sketelon/Sketelon.h"
#include "../GameObjects/Enemies/Sketelon/Mummies.h"
#include "../GameObjects/Enemies/Sketelon/BoneMummies.h"

GameMap::GameMap(char* filePath)
{
	m_apples = new unordered_set<Apple*>();
	m_enemies = new unordered_set<Entity*>();
	m_weapons = new unordered_set<Entity*>();
	LoadMap(filePath);
}

GameMap::~GameMap()
{
	delete m_pTmxMap;
	delete m_Grid;

	for (auto apples : (*m_apples))
		delete apples;
	m_apples->clear();
	delete m_apples;

	for (auto enemies : (*m_enemies))
		delete enemies;
	m_enemies->clear();
	delete m_enemies;

	for (auto weapons : (*m_weapons))
		delete weapons;
	m_weapons->clear();
	delete m_weapons;

	for (auto items : m_Items)
		delete items;
	m_Items.clear();
}

Grid* GameMap::GetGrid()
{
	return m_Grid;
}

void GameMap::LoadMap(char* filePath)
{
	m_pTmxMap = new Tmx::Map();
	m_pTmxMap->ParseFile(filePath);

	m_Grid = new Grid();

	for (size_t i = 0; i < m_pTmxMap->GetNumTilesets(); i++)
	{
		const Tmx::Tileset* tileset = m_pTmxMap->GetTileset(i);
		Sprite* sprite = new Sprite(tileset->GetImage()->GetSource().c_str(), RECT(), NULL, NULL, D3DCOLOR_XRGB(0x00, 0xff, 0xff));
		m_Tilesets.insert(pair<int, Sprite*>(i, sprite));
	}

#pragma region -OBJECTGROUP, STATIC OBJECT-

	for (size_t i = 0; i < m_pTmxMap->GetNumObjectGroups(); i++)
	{
		const Tmx::ObjectGroup* objectGroup = m_pTmxMap->GetObjectGroup(i);

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

			m_Grid->InsertEntity(entity);
		}
	}
#pragma endregion

}

void GameMap::LoadMapItems(char* filePath)
{
	m_ItemsFile = filePath;
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

			m_Items.insert(tmp);
			m_Grid->InsertEntity(tmp);
		}

		fclose(file);
	}
}

void GameMap::ReloadItems()
{
	for (auto iter : m_Items)
	{
		m_Grid->RemoveEntiy(iter);
		delete iter;
	}
	m_Items.clear();
	LoadMapItems(m_ItemsFile);
}

void GameMap::ReloadObjects()
{
	for (auto iter : (*m_enemies))
		delete iter;
	m_enemies->clear();
	LoadMapObjects(m_ObjectsFile);
}

void GameMap::LoadMapObjects(char* filePath)
{
	m_ObjectsFile = filePath;
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
				m_Grid->InsertEntity(tmp);
			}
			break;

			case Entity::TYPE_SKETELON:
			{
				auto sketelon = new Sketelon(position);
				sketelon->set_GameMap(this);
				tmp = sketelon;
			}
			break;

			case Entity::TYPE_MUMMIES:
				tmp = new Mummies(position);
				break;

			default:
				tmp = nullptr;
				break;
			}

			if (tmp != nullptr)
				m_enemies->insert(tmp);
		}

		fclose(file);
	}
}

Tmx::Map* GameMap::GetMap()
{
	return m_pTmxMap;
}

int GameMap::GetWidth()
{
	return m_pTmxMap->GetWidth() * m_pTmxMap->GetTileWidth();
}

int GameMap::GetHeight()
{
	return m_pTmxMap->GetHeight() * m_pTmxMap->GetTileHeight();
}

int GameMap::GetTileWidth()
{
	return m_pTmxMap->GetTileWidth();
}

int GameMap::GetTileHeight()
{
	return m_pTmxMap->GetTileHeight();
}

void GameMap::Update(float dt)
{

#pragma region UPDATE_ITEMS

	auto items = m_Items;

	for (auto item : items)
	{
		// neu nam ngoai camera thi delete
		if (GameCollision::AABBCheck(item->GetBound(), m_Camera->GetBound()) == false)
		{
			item->SetDraw(false);
			continue;
		}
		else
			item->SetDraw(true);

		if (item->IsDeleted() == true)
		{
			m_Grid->RemoveEntiy(item);
			m_Items.erase(item);
		}
		else
			item->Update(dt);
	}

	items.clear();

#pragma endregion

#pragma region UPDATE_APPLES

	auto apples = (*m_apples);
	for (auto apple : apples)
	{
		if (apple->IsDeleted() == true)
			m_apples->erase(apple);
		else
			apple->Update(dt);
	}
	apples.clear();

#pragma endregion

#pragma region UPDATE_ENEMIES

	auto enemies = (*m_enemies);
	for (auto enemy : enemies)
	{
		if (GameCollision::AABBCheck(enemy->GetBound(), m_Camera->GetBound()) == false)
		{
			enemy->SetDraw(false);
			if (enemy->Tag == Entity::FloatingGround || enemy->Tag == Entity::Jafar)
				enemy->Update(dt);

			continue;
		}
		else
			enemy->SetDraw(true);

		if (enemy->IsDeleted() == true)
			m_enemies->erase(enemy);
		else
			enemy->Update(dt);
	}
	enemies.clear();

#pragma endregion

#pragma region UPDATE_ENEMY_WEAPONS

	auto weapons = (*m_weapons);
	for (auto weapon : weapons)
	{
		// neu nam ngoai camera thi delete
		if (GameCollision::AABBCheck(weapon->GetBound(), m_Camera->GetBound()) == false)
			weapon->SetDraw(false);
		else
			weapon->SetDraw(true);

		if (weapon->IsDeleted() == true)
			m_weapons->erase(weapon);
		else
			weapon->Update(dt);
	}
	weapons.clear();

#pragma endregion
}

void GameMap::Draw()
{
	auto trans_x = GameGlobal::GetWidth() / 2 - m_Camera->GetPosition().x;
	auto trans_y = GameGlobal::GetHeight() / 2 - m_Camera->GetPosition().y;
	GVector2 trans = GVector2(trans_x, trans_y);

#pragma region Draw_MAP
	for (size_t i = 0; i < m_pTmxMap->GetNumTileLayers(); i++)
	{
		const Tmx::TileLayer* layer = m_pTmxMap->GetTileLayer(i);

		if (!layer->IsVisible())
			continue;

		if (i == 1)
		{
			for (auto item : m_Items)
			{
				if (item->IsDraw() == true)
					item->Draw(trans);
			}
			for (auto enemy : (*m_enemies))
			{
				if (enemy->IsDraw() == true)
					enemy->Draw(trans);
			}

			for (auto apple : (*m_apples))
			{
				if (apple->IsDraw() == true)
					apple->Draw(apple->GetPosition(), RECT(), GVector2(), trans);
			}
			for (auto weapon : (*m_weapons))
			{
				if (weapon->IsDraw() == true)
					weapon->Draw(trans);
			}
			mPlayer->Draw();
		}

		RECT srcRect;

		int tileWidth = m_pTmxMap->GetTileWidth();
		int tileHeight = m_pTmxMap->GetTileHeight();

		for (size_t m = 0; m < layer->GetHeight(); m++)
		{
			for (size_t n = 0; n < layer->GetWidth(); n++)
			{
				int tilesetIndex = layer->GetTileTilesetIndex(n, m);

				if (tilesetIndex != -1)
				{
					const Tmx::Tileset* tileSet = m_pTmxMap->GetTileset(tilesetIndex);

					int tileSetWidth = tileSet->GetImage()->GetWidth() / tileWidth;
					int tileSetHeight = tileSet->GetImage()->GetHeight() / tileHeight;

					Sprite* sprite = m_Tilesets.at(layer->GetTileTilesetIndex(n, m));

					//tile index
					int tileID = layer->GetTileId(n, m);

					int y = tileID / tileSetWidth;
					int x = tileID - y * tileSetWidth;

					srcRect.top = y * tileHeight;
					srcRect.bottom = srcRect.top + tileHeight;
					srcRect.left = x * tileWidth;
					srcRect.right = srcRect.left + tileWidth;

					GVector3 position(n * tileWidth + tileWidth / 2, m * tileHeight + tileHeight / 2, 0);

					if (m_Camera != NULL)
					{
						RECT rectObject;
						rectObject.left = position.x - tileWidth / 2;
						rectObject.top = position.y - tileHeight / 2;
						rectObject.right = rectObject.left + tileWidth;
						rectObject.bottom = rectObject.top + tileHeight;

						// neu nam ngoai camera thi khong Draw
						if (GameCollision::AABBCheck(rectObject, m_Camera->GetBound()) == false)
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
	m_Camera = camera;
}

void GameMap::InsertAppleAladdin(Apple* apple)
{
	m_apples->insert(apple);
}

void GameMap::InsertStaticObject(Entity* entity)
{
	m_Grid->InsertEntity(entity);
}

void GameMap::InsertWeapon(Entity* weapon)
{
	m_weapons->insert(weapon);
}

unordered_set<Apple*>* GameMap::GetListApples()
{
	return m_apples;
}

unordered_set<Entity*>* GameMap::GetListWeapons()
{
	return m_weapons;
}

unordered_set<Entity*>* GameMap::GetListEnemies()
{
	return m_enemies;
}

void GameMap::ClearList()
{}

void GameMap::ClearEnemiesInRegion(RECT region)
{
	for (auto enemy : (*m_enemies))
	{
		if (GameCollision::AABBCheck(enemy->GetBound(), region) == true)
			enemy->SetDelete(true);
	}
}