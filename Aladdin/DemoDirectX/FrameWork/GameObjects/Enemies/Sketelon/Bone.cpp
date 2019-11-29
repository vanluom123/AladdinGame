#include "Bone.h"
#include "../../Player/Player.h"

Bone::Bone()
{
	_anim = nullptr;
}

Bone::Bone(GVector2 position)
{
	initialize(position);
}

Bone::~Bone()
{
}

void Bone::initialize(GVector2 position)
{
	_anim = new Animation(eID::BONE, "bone", 4, 0.15f);
	isDeleted = false;
	isDestroy = false;
	isDraw = true;
	this->Tag = TYPE_BONE;
	SetPosition(position);
	SetWidth(_anim->GetSprite()->GetWidth());
	SetHeight(_anim->GetSprite()->GetHeight());
}

void Bone::Draw(D3DXVECTOR2 trans)
{
	_anim->Draw(GetPosition(), RECT(), GVector2(), trans);
}

RECT Bone::GetBound()
{
	auto pivot = _anim->GetAnchorPoint();
	RECT r = RECT();
	if (pivot.x == -1 && pivot.y == -1)
	{
		r.left = posX - _anim->GetSprite()->GetWidth() / 2;
		r.top = posY - _anim->GetSprite()->GetHeight();
		r.right = r.left + _anim->GetSprite()->GetWidth();
		r.bottom = r.top + _anim->GetSprite()->GetHeight();
	}
	else
	{
		r.left = posX - pivot.x;
		r.top = posY - pivot.y;
		r.right = r.left + _anim->GetSprite()->GetWidth();
		r.bottom = r.top + _anim->GetSprite()->GetHeight();
	}

	return r;
}

void Bone::Update(float dt)
{
	_anim->Update(dt);
}

void Bone::OnCollision(Entity* impactor, CollisionReturn data, SideCollisions side)
{
	if (impactor->Tag == Entity::Aladdin)
	{
		auto aladdin = static_cast<Player*>(impactor);

	}
}