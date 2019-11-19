#include "Viewport.h"
#include "GameGlobal.h"
#include "../GameObjects/Player/Player.h"
#include "GameMap.h"


Viewport::Viewport(int width, int height)
{
	_width = width;
	_height = height;
	_position = GVector3(0, 0, 0);
}


Viewport::~Viewport()
{
}

void Viewport::SetPosition(float x, float y)
{
	SetPosition(GVector3(x, y, 0));
}

void Viewport::SetPosition(GVector3 pos)
{
	_position = pos;
}

GVector3 Viewport::GetPosition()
{
	return _position;
}

RECT Viewport::GetBound()
{
	RECT bound;

	bound.left = long(_position.x - _width / 2);
	bound.right = bound.left + _width;
	bound.top = long(_position.y - _height / 2);
	bound.bottom = bound.top + _height;

	return bound;
}

GVector2 Viewport::getTranslate()
{
	return GVector2(GameGlobal::GetWidth() / 2 - _position.x, GameGlobal::GetHeight() / 2 - _position.y);
}

void Viewport::checkViewportWithMapWorld(Player* pPlayer, GameMap* pGameMap)
{
	this->SetPosition(pPlayer->getPosition());

	if (this->GetBound().left < 0)
	{
		//vi position cua camera ma chinh giua camera
		//luc nay o vi tri goc ben trai cua the gioi thuc
		this->SetPosition(this->GetWidth() / 2, this->GetPosition().y);
	}

	if (this->GetBound().right > pGameMap->GetWidth())
	{
		//luc nay cham goc ben phai cua the gioi thuc
		this->SetPosition(pGameMap->GetWidth() - this->GetWidth() / 2,
			this->GetPosition().y);
	}

	if (this->GetBound().top < 0)
	{
		//luc nay cham goc tren the gioi thuc
		this->SetPosition(this->GetPosition().x, this->GetHeight() / 2);
	}

	if (this->GetBound().bottom > pGameMap->GetHeight())
	{
		//luc nay cham day cua the gioi thuc
		this->SetPosition(this->GetPosition().x,
			pGameMap->GetHeight() - this->GetHeight() / 2);
	}
}

int Viewport::GetWidth()
{
	return _width;
}

int Viewport::GetHeight()
{
	return _height;
}