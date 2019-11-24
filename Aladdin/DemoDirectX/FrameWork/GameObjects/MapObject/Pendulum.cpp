#include "Pendulum.h"

Pendulum::~Pendulum()
{
}

Pendulum::Pendulum()
{
}

Pendulum::Pendulum(GVector3 position)
{
	init(position);
}

Pendulum::Pendulum(GVector2 position)
{
	init(GVector3(position.x, position.y, 0));
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

void Pendulum::Draw(GVector3 position, RECT sourceRect, GVector2 scale, GVector2 transform, float angle, GVector2 rotationCenter, D3DXCOLOR colorKey)
{
	mAnimation->Draw(position, sourceRect, scale, transform, angle, rotationCenter, colorKey);
}

void Pendulum::Draw(GVector3 transform)
{
}

void Pendulum::Draw(GVector2 transform)
{
}

void Pendulum::OnCollision(Entity* impactor, Entity::CollisionReturn data, Entity::SideCollisions side)
{
}

bool Pendulum::init(GVector3 position)
{
	this->Tag = Entity::EntityTypes::Pendulum;
	mAnimation = new Animation(eID::Pendulum, "Pendulum", 15, 0.2, NULL);
	isDestroy = false;
	SetPosition(position);
	Entity::SetWidth(mAnimation->GetSprite()->GetWidth());
	Entity::SetHeight(mAnimation->GetSprite()->GetHeight());
	return false;
}

void Pendulum::OnSetPosition(GVector3 poks)
{
	mAnimation->SetPosition(poks);
}
