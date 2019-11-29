#include "Sketelon.h"

Sketelon::Sketelon(GVector2 position)
{
	initialize(position);
}

Sketelon::~Sketelon()
{
	delete _anim;
}

void Sketelon::initialize(GVector2 position)
{
	_anim = new Animation(eID::SKETELON, "sketelon", 20, 0.15f);
	Tag = Entity::TYPE_SKETELON;
	isDestroy = false;
	isDeleted = false;
	isDraw = true;
	SetPosition(position);
	SetWidth(_anim->GetSprite()->GetWidth());
	SetHeight(_anim->GetSprite()->GetHeight());
}

void Sketelon::Draw(GVector2 trans)
{
	_anim->Draw(GetPosition(), RECT(), GVector2(), trans);
}

RECT Sketelon::GetBound()
{
	auto anchor = _anim->GetAnchorPoint();
	RECT r = RECT();
	if (anchor.x == -1 && anchor.y == -1)
	{

	}
	else
	{
		r.left = posX - anchor.x;
		r.top = posY - anchor.y;
		r.right = r.left + _anim->GetSprite()->GetWidth();
		r.bottom = r.top + _anim->GetSprite()->GetHeight();
	}

	return r;
}

void Sketelon::Update(float dt)
{
	_anim->Update(dt);
}

void Sketelon::OnCollision(Entity* impactor, CollisionReturn data, SideCollisions side)
{
	if (impactor->Tag == Entity::Aladdin)
	{

	}
}