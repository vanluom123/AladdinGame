#include "BoneMummies.h"

BoneMummies::BoneMummies(GVector2 position)
{
	initialize(position);
}

BoneMummies::~BoneMummies()
{
	delete _anim;
}

void BoneMummies::initialize(GVector2 position)
{
	Tag = Entity::TYPE_MUMMIES_BONE;
	_anim = new Animation(eID::BONE, "bone_", 3, 0.15f);
	isDraw = true;
	isDeleted = false;
	isDestroy = false;
	SetPosition(position);
	SetWidth(_anim->GetSprite()->GetWidth());
	SetHeight(_anim->GetSprite()->GetHeight());
}

void BoneMummies::Draw(GVector2 trans)
{
	_anim->Draw(GetPosition(), RECT(), GVector2(), trans);
}

RECT BoneMummies::GetBound()
{
	auto anchor = _anim->GetAnchorPoint();
	RECT r = RECT();
	if (anchor.x == -1 && anchor.y == -1)
	{
		r.left = posX - _anim->GetSprite()->GetWidth() / 2;
		r.top = posY - _anim->GetSprite()->GetHeight();
		r.right = r.left + _anim->GetSprite()->GetWidth();
		r.bottom = r.top + _anim->GetSprite()->GetHeight();
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

void BoneMummies::Update(float dt)
{
	_anim->Update(dt);
}

void BoneMummies::OnCollision(Entity* impactor, CollisionReturn data, SideCollisions side)
{
}
