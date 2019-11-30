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
	_debug->setColor(D3DCOLOR_XRGB(255, 255, 255));
	_debug->DrawRect(this->GetBound(), nullptr);
}

RECT Bone::GetBound()
{
	RECT bound;
	bound.left = posX - width / 2;
	bound.top = posY - height;
	bound.right = bound.left + width;
	bound.bottom = bound.top + height;

	return bound;
}

void Bone::Update(float dt)
{
	OnCollision(_gameMap->mPlayer);
	_anim->Update(dt);
}

void Bone::OnCollision(Entity* impactor)
{
	if (impactor->Tag == Entity::Aladdin)
	{
		auto player = (Player*)impactor;
		if (player->getState() == PlayerState::Die)
			return;

		auto check = GameCollision::AABBCheck(player->GetBound(), GetBound());
		if (check)
		{
			if (player->GetTimeImmortal() <= 0)
				player->TakeDamage(1);
		}
	}
}

void Bone::set_GameMap(GameMap* val)
{
	_gameMap = val;
}
