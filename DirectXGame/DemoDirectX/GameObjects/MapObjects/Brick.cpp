#include "Brick.h"
#include "../../GameComponents/GameGlobal.h"
#include "../../GameDefines/GameDefine.h"


Brick::Brick()
{
	this->mAnimation = nullptr;
}

Brick::~Brick()
{
	delete this->mAnimation;
}

bool Brick::init(GVector3 position)
{
	this->mAnimation = new Animation(eID::BRICK, this->FileName(), this->TotalFrame(), this->SecondPerFrame());

	this->setPosition(position);

	Entity::SetWidth(this->mAnimation->getSprite()->getWidth());
	Entity::SetHeight(this->mAnimation->getSprite()->getHeight());

	return true;
}

void Brick::OnSetPosition(GVector3 pos)
{
	this->mAnimation->setPosition(pos);
}

void Brick::Update(float dt)
{
	this->mAnimation->update(dt);
}

void Brick::Draw(GVector3 position, RECT sourceRect, GVector2 scale, GVector2 transform, float angle, GVector2 rotationCenter, D3DXCOLOR colorKey)
{
	this->mAnimation->draw(position, sourceRect, scale, transform, angle, rotationCenter, colorKey);
}

void Brick::Draw(GVector2 transform)
{
	this->mAnimation->draw(GVector2(transform));
}
