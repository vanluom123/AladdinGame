#include "Trap.h"
#include "../Player/Player.h"

Trap::Trap(D3DXVECTOR2 position)
{
	this->SetDelete(false);
	this->SetDraw(true);
	this->SetDestroy(false);
	_time = 0;
	_anim = new Animation(eID::BRICK_TRAP_PENDULUM, "Trap_", 6, 0.25f);
	_status = 0;
	Tag = Entity::TYPE_TRAP;
	SetPosition(position);
	SetWidth(_anim->GetSprite()->GetWidth());
	SetHeight(_anim->GetSprite()->GetHeight());
}

Trap::~Trap()
{
	delete _anim;
}

void Trap::Update(float dt)
{
	_time += dt;

	if (_status == 0)
	{
		_anim->SetTimePerFrame(0.07f);

		if (_time > 1.4f)
		{
			_anim->Update(dt);
			if (_anim->mCurrentIndex == 6)
			{
				_status = 1;
				_time = 0;
			}
		}
	}
	else if (_status == 1)
	{
		_anim->SetTimePerFrame(0.25f);
		if (_time > 0.1f)
		{
			_anim->Update(dt, true);
			if (_anim->mCurrentIndex == 1)
			{
				_status = 0;
				_time = 0;
			}
		}
	}
}

void Trap::Draw(D3DXVECTOR2 trans)
{
	_anim->Draw(GetPosition(), RECT(), D3DXVECTOR2(), trans);
}

void Trap::Draw(D3DXVECTOR3 position, RECT sourceRect, D3DXVECTOR2 scale, D3DXVECTOR2 transform, float angle, D3DXVECTOR2 rotationCenter, D3DXCOLOR colorKey)
{
	_anim->Draw(position, sourceRect, scale, transform, angle, rotationCenter, colorKey);
}

RECT Trap::GetBound()
{
	RECT r = RECT();

	POINT tmp = _anim->GetSprite()->GetAnchorPoint();
	if (tmp.x == -1 && tmp.y == -1)
	{
		r.left = posX - 49 / 2;
		r.top = posY - 35 / 2;
		r.right = r.left + 49;
		r.bottom = r.top + 35;
	}
	else
	{
		r.left = posX - tmp.x;
		r.top = posY - tmp.y;
		r.right = r.left + 49;
		r.bottom = r.top + 35;
	}

	return r;
}

void Trap::OnCollision(Entity* impactor, CollisionReturn data, SideCollisions side)
{
	if (_anim->mCurrentIndex != 6)
		return;

	if (impactor->Tag == Entity::Aladdin)
	{
		auto player = static_cast<Player*>(impactor);
		switch (player->getState())
		{
		case PlayerState::HangCut:
		case PlayerState::Cut:
		case PlayerState::DownCut:
		case PlayerState::JumpCut:
		case PlayerState::RunCut:
		case PlayerState::UpCut:
		{
			auto rCollide = GameCollision::rectCollide(this->GetBound(), player->GetBoundWeapon());
			if (rCollide.IsCollided)
				return;

			if (player->GetTimeImmortal() <= 0)
				player->TakeDamage(1);
		}
		break;
		default:
			if (player->GetTimeImmortal() <= 0)
				player->TakeDamage(1);
			break;
		}
	}
}
