#include "Pendulum.h"

Pendulum::~Pendulum()
{
}

Pendulum::Pendulum()
{
}

Pendulum::Pendulum(D3DXVECTOR3 position, GameMap * gameMap)
{
	init(position, gameMap);
}

Pendulum::Pendulum(D3DXVECTOR2 position, GameMap * gameMap)
{
	init(D3DXVECTOR3(position.x, position.y, 0), gameMap);
}

void Pendulum::Update(float dt)
{

}

RECT Pendulum::GetBound()
{
	RECT bound;
	bound.left = this->GetPosition().x - this->GetWidth() / 4;
	bound.right = bound.left + this->GetWidth() / 2;
	bound.top = this->GetPosition().y - mAnimation->GetSprite()->GetHeight();
	bound.bottom = bound.top + mAnimation->GetSprite()->GetHeight();
	return bound;
}

void Pendulum::Draw(D3DXVECTOR3 position, RECT sourceRect, D3DXVECTOR2 scale, D3DXVECTOR2 transform, float angle, D3DXVECTOR2 rotationCenter, D3DXCOLOR colorKey)
{
	mAnimation->Draw(position, sourceRect, scale, transform, angle, rotationCenter, colorKey);
}

void Pendulum::Draw(D3DXVECTOR3 transform)
{
}

void Pendulum::Draw(D3DXVECTOR2 transform)
{
}

void Pendulum::OnCollision(Entity * impactor, Entity::CollisionReturn data, Entity::SideCollisions side)
{
}

bool Pendulum::init(D3DXVECTOR3 position, GameMap *gameMap)
{
	this->Tag = Entity::EntityTypes::Pendulum;
	mAnimation = new Animation(eID::Pendulum, "Pendulum", 15, 0.2, NULL);
	isDestroy = false;
	SetPosition(position);
	Entity::SetWidth(mAnimation->GetSprite()->GetWidth());
	Entity::SetHeight(mAnimation->GetSprite()->GetHeight());
	this->gameMap = gameMap;
	return false;
}

void Pendulum::OnSetPosition(D3DXVECTOR3 poks)
{
	mAnimation->SetPosition(poks);
}
