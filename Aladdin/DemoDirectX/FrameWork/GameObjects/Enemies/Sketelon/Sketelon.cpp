#include "Sketelon.h"
#include "../../../GameComponents/GameCollision.h"
#include "../../Player/Player.h"

Sketelon::Sketelon(GVector2 position)
{
	initialize(position);
}

Sketelon::~Sketelon()
{
	delete _anim;
	delete _animDeath;
	delete _currentAnim;
	delete _bone;
}

void Sketelon::initialize(GVector2 position)
{
	HP = 3;
	Tag = Entity::TYPE_SKETELON;
	_anim = new Animation(eID::SKETELON, "sketelon", 20, 0.5f);
	_animDeath = new Animation(eID::EXPLOSION, "Explosion1st", 4, 0.35f);
	_currentAnim = _anim;
	_bone = new Bone();
	isDestroy = false;
	isDeleted = false;
	isDraw = true;
	SetPosition(position);
	SetWidth(_anim->GetSprite()->GetWidth());
	SetHeight(_anim->GetSprite()->GetHeight());
}

void Sketelon::Draw(GVector2 trans)
{
	for (int i = 0; i < 8; i++)
		_bone->Draw(_position_explode[i], trans);

	if (_currentAnim->IsStop() == false && _status != eSke_Explode)
		_currentAnim->Draw(GetPosition(), RECT(), GVector2(), trans);
}

RECT Sketelon::GetBound()
{
	auto anchor = _anim->GetAnchorPoint();
	RECT bound;
	bound.left = posX - anchor.x;
	bound.top = posY - anchor.y;
	bound.right = bound.left + _anim->GetSprite()->GetWidth();
	bound.bottom = bound.top + _anim->GetSprite()->GetHeight();

	return bound;
}

void Sketelon::CapNhat_PhatNo(float dt)
{
	for (int i = 0; i < 8; i++)
	{
		D3DXVec2Normalize(&_velocity_explode[i], &_velocity_explode[i]);
		_position_explode[i].x += _velocity_explode[i].x * dt * 50;
		_position_explode[i].y += _velocity_explode[i].y * dt * 50;
	}

	_bone->Update(dt);
}

void Sketelon::PhatNo()
{
	vx = 0.0f;
	vy = 0.0f;
	for (int i = 0; i < 8; i++)
	{
		_position_explode[i].x = posX;
		_position_explode[i].y = posY;
	}
}

void Sketelon::set_GameMap(GameMap* val)
{
	_gameMap = val;
	_bone->set_GameMap(val);
}

void Sketelon::Update(float dt)
{
	if (_currentAnim->IsStop() == false && _status != eSke_Explode)
		_currentAnim->Update(dt);
	Entity::Update(dt);

	switch (_status) {
	case eSke_Normal:
	{
		if (_currentAnim->mCurrentIndex == 20)
		{
			_status = eSke_Explode;
			PhatNo();
		}
	}
	break;
	case eSke_Explode:
		CapNhat_PhatNo(dt);
		break;
	case eSke_Death:
	{
		_currentAnim = _animDeath;
		_currentAnim->SetStop(true);
	}
	break;
	default: break;
	}
}

void Sketelon::OnCollision(Entity* impactor, CollisionReturn data, SideCollisions side)
{
	if (isDeleted == true || _status == eSke_Explode)
		return;

	if (HP <= 0) {
		_status = eSke_Death;
		return;
	}

	if (impactor->Tag == Entity::Aladdin)
	{
		switch (((Player*)impactor)->getState())
		{
		case PlayerState::HangCut:
		case PlayerState::Cut:
		case PlayerState::DownCut:
		case PlayerState::JumpCut:
		case PlayerState::RunCut:
		case PlayerState::UpCut:
		{
			auto r = GameCollision::rectCollide(((Player*)impactor)->GetBoundWeapon(), GetBound());
			if (r.IsCollided) {
				TakeDamage(1);
			}
		}
		break;
		default:
		{
			auto r = GameCollision::rectCollide(((Player*)impactor)->GetBoundBody(), GetBound());
			if (r.IsCollided)
			{
				if (((Player*)impactor)->GetTimeImmortal() <= 0)
					((Player*)impactor)->TakeDamage(1);
			}
		}
		break;
		}
	}
}