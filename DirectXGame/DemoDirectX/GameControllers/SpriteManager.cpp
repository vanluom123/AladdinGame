/*
HOW TO USE:
spritemanager is basic class to manage load and release sprite and texture.
It load all image you have, if you want to use sprite any where, just getSprite from SpriteManager.
if you dont want you use this image any more, you can call release.

everytime use this, you have to call SpriteManager::getInstance() first.
this is Singleton format.
Seemore about Singleton: http://www.stdio.vn/articles/read/224/singleton-pattern

you can call directly: SpriteManager::getInstance()->DOSOMETHING()
or reference it:
SpriteManager* _spritemanager;
...
_spritemanager = SpriteManager::getInstance();
...
_spritemanager->DOSOMETHING()

Next, you should call loadresource(LPD3DXSPRITE spriteHandle) in Game::loadresource().
It will load all your image from file to memory.
Remember: Insert your code to loadresource to load image from file.

If you want object reference to Sprite. call:
SpriteManager::getInstance()->getSprite(eID::OBJECT_ID)
what is eID::OBJECT_ID ?
in define.h you can insert element to eID.

OK. Now you have sprite to draw.

If you dont want to use this sprite any more, call releaseSprite or releaseTexture
they often are called in object::release

Call SpriteManager::release() in game:release() to clean all memory.
*/
#include "SpriteManager.h"


SpriteManager* SpriteManager::_instance = nullptr;
SpriteManager::SpriteManager(void)
{
	// do nothing.
}

SpriteManager::~SpriteManager(void)
{
	for (auto spr = _listSprite.begin(); spr != _listSprite.end(); ++spr)
	{
		delete spr->second;			// delete sprite
	}
	if (_listSprite.empty() == false)
		_listSprite.clear();		// remove all from MAP
}

void SpriteManager::loadResource()
{
	Sprite* sp = NULL;

	// PLAYER
	sp = new Sprite("Resources/mario/sketelonPivot.png", RECT(), 0, 0, D3DCOLOR_XRGB(255, 255, 255));
	_listSprite.insert(pair<eID, Sprite*>(eID::PLAYER, sp));
	this->loadSpriteInfo(eID::PLAYER, "Resources//mario//sketelonPivot.txt", true);

	// BRICK
	sp = new Sprite("Resources/goldbrick_normalbrick.png", RECT(), 0, 0, D3DCOLOR_XRGB(255, 255, 255));
	_listSprite.insert(pair<eID, Sprite*>(eID::BRICK, sp));
	this->loadSpriteInfo(eID::BRICK, "Resources//goldbrick.txt");
}



Sprite* SpriteManager::getSprite(eID id)
{
	Sprite* it = this->_listSprite.find(id)->second;
	return new Sprite(*it);			// get the copy version of Sprite
}

RECT SpriteManager::getSourceRect(eID id, string name)
{
	return _sourceRectList[id][name];
}
POINT SpriteManager::getAnchorPoint(eID id, string name)
{
	return _anchorPointList[id][name];
}

void SpriteManager::loadSpriteInfo(eID id, const char* fileInfoPath)
{
	FILE* file;
	file = fopen(fileInfoPath, "r");

	if (file)
	{
		while (!feof(file))
		{
			RECT rect;
			char name[100];
			fgets(name, 100, file);

			(void)fscanf(file, "%s %d %d %d %d", &name, &rect.left, &rect.top, &rect.right, &rect.bottom);

			//Đã chỉnh sửa để phù hợp với file : Left Top Width Height
			rect.right += rect.left;
			rect.bottom += rect.top;
			_sourceRectList[id][string(name)] = rect;
			POINT tmp;
			tmp.x = -1;
			tmp.y = -1;
			_anchorPointList[id][string(name)] = tmp;
		}

		fclose(file);
	}
}

void SpriteManager::loadSpriteInfo(eID id, const char* fileInfoPath, bool isHaveAnchorPoint)
{
	FILE* file;
	file = fopen(fileInfoPath, "r");

	if (file)
	{
		while (!feof(file))
		{
			RECT rect;
			POINT anchorPoint;
			char name[100];
			fgets(name, 100, file);

			(void)fscanf(file, "%s %d %d %d %d %d %d", &name, &rect.left, &rect.top, &rect.right, &rect.bottom, &anchorPoint.x, &anchorPoint.y);

			//Đã chỉnh sửa để phù hợp với file : Left Top Width Height
			rect.right += rect.left;
			rect.bottom += rect.top;
			_sourceRectList[id][string(name)] = rect;
			_anchorPointList[id][string(name)] = anchorPoint;
		}

		fclose(file);
	}
}


void SpriteManager::releaseSprite(eID id)
{
	Sprite* it = this->_listSprite.find(id)->second;
	delete it;							// delete the sprite only, dont relase image
	this->_listSprite.erase(id);		// erase funciotn only remove the pointer from MAP, dont delete it.
}

void SpriteManager::releaseTexture(eID id)
{
	Sprite* spr = this->_listSprite.find(id)->second;				// release image
	delete spr;
	this->_listSprite.erase(id);		// erase funciotn only remove the pointer from MAP, dont delete it.
}

SpriteManager* SpriteManager::getInstance()
{
	if (_instance == nullptr)
		_instance = new SpriteManager();
	return _instance;
}

void SpriteManager::release()
{
	delete _instance;			// _instance is static attribute, only static function can delete it.
	_instance = nullptr;
}