#include "Bone.h"
#include "../../../GameComponents/GameMap.h"

Bone::Bone(GVector2 position)
{
	initialize(position);
}

Bone::Bone()
{
	Tag = TYPE_BONE;
	_debug = new GameDebugDraw();
	_anim = new Animation(eID::BONE, "bone", 4, 0.25f);
	isDeleted = false;
	isDestroy = false;
	isDraw = true;
	SetWidth(_anim->GetSprite()->GetWidth());
	SetHeight(_anim->GetSprite()->GetHeight());
}

Bone::~Bone()
{
	delete _anim;
	delete _debug;
}

void Bone::initialize(GVector2 position)
{
	Tag = TYPE_BONE;
	_anim = new Animation(eID::BONE, "bone", 4, 0.25f);
	isDeleted = false;
	isDestroy = false;
	isDraw = true;
	SetPosition(position);
	SetWidth(_anim->GetSprite()->GetWidth());
	SetHeight(_anim->GetSprite()->GetHeight());
}

void Bone::Draw(GVector2 trans)
{
	_anim->Draw(GetPosition(), RECT(), GVector2(), trans);
}

void Bone::Draw(GVector2 position, GVector2 trans)
{
	SetPosition(position);
	_anim->Draw(GetPosition(), RECT(), GVector2(), trans);
	_debug->DrawRect(this->GetBound(), nullptr);
}

RECT Bone::GetBound()
{
	RECT bound;
	bound.left = posX - _anim->GetSprite()->GetWidth() / 2;
	bound.top = posY - _anim->GetSprite()->GetHeight();
	bound.right = bound.left + _anim->GetSprite()->GetWidth();
	bound.bottom = bound.top + _anim->GetSprite()->GetHeight();

	return bound;
}

void Bone::Update(float dt)
{
	_anim->Update(dt);
}