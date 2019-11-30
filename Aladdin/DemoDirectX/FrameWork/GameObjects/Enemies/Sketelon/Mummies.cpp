#include "Mummies.h"

Mummies::Mummies(GVector2 position)
{
	initialize(position);
}

Mummies::~Mummies()
{
	delete _anim;
}

void Mummies::initialize(GVector2 position)
{
	_anim = new Animation(eID::MUMMIES, "mummies", 12, 0.15f);
	Tag = Entity::TYPE_MUMMIES;
	isDestroy = false;
	isDeleted = false;
	isDraw = true;
	SetPosition(position);
	SetWidth(_anim->GetSprite()->GetWidth());
	SetHeight(_anim->GetSprite()->GetHeight());
}

void Mummies::Draw(GVector2 trans)
{
	_anim->Draw(GetPosition(), RECT(), GVector2(), trans);
}

void Mummies::Update(float dt)
{
	_anim->Update(dt);
}

void Mummies::OnCollision(Entity* impactor, CollisionReturn data, SideCollisions side)
{
}

RECT Mummies::GetBound()
{
	auto anchor = _anim->GetAnchorPoint();
	RECT bound = RECT();
	if (anchor.x == -1 && anchor.y == -1)
	{

	}
	else
	{
		bound.left = posX - anchor.x;
		bound.top = posY - anchor.y;
		bound.right = bound.left + _anim->GetSprite()->GetWidth();
		bound.bottom = bound.top + _anim->GetSprite()->GetHeight();
	}

	return bound;
}
