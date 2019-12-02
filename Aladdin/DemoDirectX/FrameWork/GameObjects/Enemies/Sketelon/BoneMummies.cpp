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
	_anim = new Animation(eID::BONE, "BoneMummies", 3, 0.25f);
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
	RECT bound = RECT();
	if (anchor.x == -1 && anchor.y == -1)
	{
		bound.left = posX - _anim->GetSprite()->GetWidth() / 2;
		bound.top = posY - _anim->GetSprite()->GetHeight();
		bound.right = bound.left + _anim->GetSprite()->GetWidth();
		bound.bottom = bound.top + _anim->GetSprite()->GetHeight();
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

void BoneMummies::Update(float dt)
{
	_anim->Update(dt);
}