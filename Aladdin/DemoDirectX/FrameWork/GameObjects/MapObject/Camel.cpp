#include "Camel.h"
#include "../../GameComponents/GameGlobal.h"
#include "../../GameComponents/GameCollision.h"

Camel::Camel()
{

}

Camel::~Camel()
{

}

bool Camel::init(GVector3 position)
{
	this->Tag = Entity::EntityTypes::Camel;
	mAnimation = new Animation(eID::ALADDINSP, "Camel", 1, 0.1, NULL);
	isDestroy = false;
	SetPosition(position);
	Entity::SetWidth(mAnimation->GetSprite()->GetWidth());
	Entity::SetHeight(5);
	return true;
}

Camel::Camel(GVector3 position)
{
	init(position);
}

Camel::Camel(GVector2 position)
{
	init(GVector3(position.x, position.y, 0));
}


void Camel::OnSetPosition(GVector3 pos)
{
	mAnimation->SetPosition(pos);
}

void Camel::Update(float dt)
{
	if (this->isDestroy == true)
	{
		delete mAnimation;
		mAnimation = new Animation(eID::ALADDINSP, "CamelAttack", 6, 0.2f, NULL);
		isDestroy = false;
	}
	if (mAnimation->mCurrentIndex == 6 && canThrow == false)
	{
		delete mAnimation;
		mAnimation = new Animation(eID::ALADDINSP, "Camel", 1, 0.1f, NULL);
	}
	if (mAnimation->mCurrentIndex == 6)
	{
		if (canThrow == true)
		{
			class Apple* tmp = new class Apple(GVector2(this->posX + 90, this->posY - 20), true);
			tmp->SetVx(125.0f);
			GameMap::GetInstance()->InsertAppleAladdin(tmp);
		}
		canThrow = false;
	}
	mAnimation->Update(dt);
}

void Camel::Draw(GVector3 position, RECT sourceRect, GVector2 scale, GVector2 transform, float angle, GVector2 rotationCenter, D3DXCOLOR colorKey)
{
	mAnimation->Draw(position, sourceRect, scale, transform, angle, rotationCenter, colorKey);
}


void Camel::OnCollision(Entity* impactor, Entity::CollisionReturn data, Entity::SideCollisions side)
{
	canThrow = true;
}

void Camel::Draw(GVector3 transform)
{

}

void Camel::Draw(GVector2 transform)
{
	mAnimation->Draw(this->GetPosition(), RECT(), GVector2(1, 1), transform);
}

RECT Camel::GetBound()
{
	RECT bound;
	bound.left = this->GetPosition().x - this->GetWidth() / 4;
	bound.right = bound.left + this->GetWidth() / 2;
	bound.top = this->GetPosition().y - mAnimation->GetSprite()->GetHeight();
	bound.bottom = bound.top + mAnimation->GetSprite()->GetHeight();
	return bound;
}