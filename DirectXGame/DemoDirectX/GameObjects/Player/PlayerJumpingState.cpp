#include "PlayerJumpingState.h"
#include "PlayerStandingState.h"
#include "PlayerFallingState.h"
#include "../../GameComponents/GameCollision.h"
#include "../../GameDefines/GameDefine.h"

PlayerJumpingState::PlayerJumpingState(PlayerData playerData)
{
	_playerData = playerData;
	_playerData._player->SetVy(Define::PLAYER_MIN_JUMP_VELOCITY);

	_acceleratorY = 15.0f;
	_acceleratorX = 14.0f;

	_noPressed = false;
}


PlayerJumpingState::~PlayerJumpingState()
{

}

void PlayerJumpingState::update(float dt)
{
	_playerData._player->AddVy(_acceleratorY);

	if (_playerData._player->GetVy() >= 0)
	{
		_playerData._player->setState(new PlayerFallingState(_playerData));

		return;
	}

	if (_noPressed)
	{
		if (_playerData._player->getMoveDirection() == Player::MoveToLeft)
		{
			if (_playerData._player->GetVx() < 0)
			{
				_playerData._player->AddVx(_acceleratorX);

				if (_playerData._player->GetVx() > 0)
					_playerData._player->SetVx(0);
			}
		}
		else if (_playerData._player->getMoveDirection() == Player::MoveToRight)
		{
			if (_playerData._player->GetVx() > 0)
			{
				_playerData._player->AddVx(-_acceleratorX);

				if (_playerData._player->GetVx() < 0)
					_playerData._player->SetVx(0);
			}
		}
	}
}

void PlayerJumpingState::keyboardEventHandler(std::map<int, bool> keys)
{
	if (keys[VK_RIGHT])
	{
		_playerData._player->Reverse = false;

		if (_playerData._player->GetVx() < Define::PLAYER_MAX_RUNNING_SPEED)
		{
			_playerData._player->AddVx(_acceleratorX);

			if (_playerData._player->GetVx() >= Define::PLAYER_MAX_RUNNING_SPEED)
			{
				_playerData._player->SetVx(Define::PLAYER_MAX_RUNNING_SPEED);
			}
		}

		_noPressed = false;
	}
	else if (keys[VK_LEFT])
	{
		_playerData._player->Reverse = true;

		if (_playerData._player->GetVx() > -Define::PLAYER_MAX_RUNNING_SPEED)
		{
			_playerData._player->AddVx(-_acceleratorX);

			if (_playerData._player->GetVx() < -Define::PLAYER_MAX_RUNNING_SPEED)
			{
				_playerData._player->SetVx(-Define::PLAYER_MAX_RUNNING_SPEED);
			}
		}

		_noPressed = false;
	}
	else
		_noPressed = true;
}

void PlayerJumpingState::onCollide(Entity* impactor, Entity::eSide side, Entity::CollisionReturn data)
{
	switch (side)
	{
	case Entity::Left:
	{
		_playerData._player->AddPosition(data._RegionCollision.right - data._RegionCollision.left, 0);
		_playerData._player->SetVx(0);
		break;
	}

	case Entity::Right:
	{
		_playerData._player->AddPosition(-(data._RegionCollision.right - data._RegionCollision.left), 0);
		_playerData._player->SetVx(0);
		break;
	}

	case Entity::Top:
	{
		_playerData._player->AddPosition(0, data._RegionCollision.bottom - data._RegionCollision.top);
		_playerData._player->SetVy(0);
		break;
	}

	case Entity::Bottom:
	{
		_playerData._player->AddPosition(0, -(data._RegionCollision.bottom - data._RegionCollision.top));
	}

	default:
		break;
	}
}

PlayerState::StateName PlayerJumpingState::GetState()
{
	return PlayerState::Jumping;
}

void PlayerJumpingState::onCollision(Entity::eSide side)
{
	switch (side) {

	case Entity::Left:
	case Entity::Right:
		_playerData._player->SetVx(0);
		break;

	case Entity::Top:
		_playerData._player->SetVy(0);
		_playerData._player->setState(new PlayerFallingState(_playerData));
		break;

	default: break;
	}
}
