#include "Pendulum.h"

Pendulum::~Pendulum()
{
	delete _anim;
}

Pendulum::Pendulum(GVector2 position)
{
	init(position);
}

void Pendulum::Update(float dt)
{
	_time += dt;

	if (_status == 0)
	{
		if (_time > 0.3f)
		{
			_anim->Update(dt);
			if (_anim->mCurrentIndex == 15)
			{
				_time = 0;
				_status = 1;
			}
		}
	}
	else if (_status == 1)
	{
		if (_time > 0.3f)
		{
			_anim->Update(dt, true);
			if (_anim->mCurrentIndex == 1)
			{
				_time = 0;
				_status = 0;
			}
		}
	}
}

RECT Pendulum::GetBound()
{
	POINT pivot = _anim->GetAnchorPoint();
	RECT r = RECT();

	if (pivot.x == -1 && pivot.y == -1)
	{
		r.left = posX - 33 / 2;
		r.top = posY - 52;
		r.right = r.left + 33;
		r.bottom = r.top + 52;
	}
	else
	{
		r.left = posX - pivot.x;
		r.top = posY - pivot.y;
		r.right = r.left + 33;
		r.bottom = r.top + 52;
	}

	return r;
}

void Pendulum::Draw(GVector3 position, RECT sourceRect, GVector2 scale, GVector2 transform, float angle, GVector2 rotationCenter, D3DXCOLOR colorKey)
{
	_anim->Draw(position, sourceRect, scale, transform, angle, rotationCenter, colorKey);
}

void Pendulum::Draw(GVector2 transform)
{
	_anim->Draw(GetPosition(), RECT(), D3DXVECTOR2(), transform);
}

void Pendulum::OnCollision(Entity* impactor, CollisionReturn data, SideCollisions side)
{
	if ((_anim->mCurrentIndex != 15) && (_anim->mCurrentIndex != 14))
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
			auto rCollide = GameCollision::RecteAndRect(this->GetBound(), player->GetBoundWeapon());
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

void Pendulum::init(GVector2 position)
{
	Tag = Entity::TYPE_PENDULUM;
	_anim = new Animation(eID::BRICK_TRAP_PENDULUM, "pendulum_", 15, 0.15f);
	isDestroy = false;
	isDeleted = false;
	isDraw = true;
	SetPosition(position);
	SetWidth(_anim->GetSprite()->GetWidth());
	SetHeight(_anim->GetSprite()->GetHeight());
}