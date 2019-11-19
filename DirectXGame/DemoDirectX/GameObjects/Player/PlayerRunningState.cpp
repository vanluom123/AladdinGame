#include "PlayerRunningState.h"
#include "PlayerStandingState.h"
#include "PlayerFallingState.h"
#include "../../GameComponents/GameCollision.h"
#include "../../GameDefines/GameDefine.h"

const float PlayerRunningState::SPEED = 250.0f;

PlayerRunningState::PlayerRunningState(PlayerData playerData)
{
	_playerData = playerData;
	_acceleratorX = 50.0f;
	_playerData._player->setAllowMoveLeft(true);
	_playerData._player->setAllowMoveRight(true);
}


PlayerRunningState::~PlayerRunningState()
{
}

void PlayerRunningState::keyboardEventHandler(std::map<int, bool> keys)
{
	if (keys[VK_RIGHT])
	{
		if (_playerData._player->allowMoveRight())
		{
			_playerData._player->Reverse = false;
			_playerData._player->SetVx(SPEED);

			if (_playerData._player->GetVx() < Define::PLAYER_MAX_RUNNING_SPEED)
			{
				_playerData._player->AddVx(_acceleratorX);

				if (_playerData._player->GetVx() >= Define::PLAYER_MAX_RUNNING_SPEED)
					_playerData._player->SetVx(Define::PLAYER_MAX_RUNNING_SPEED);
			}
		}
	}
	else if (keys[VK_LEFT])
	{
		if (_playerData._player->allowMoveLeft())
		{
			_playerData._player->Reverse = true;
			_playerData._player->SetVx(-SPEED);

			if (_playerData._player->GetVx() > -Define::PLAYER_MAX_RUNNING_SPEED)
			{
				_playerData._player->AddVx(-_acceleratorX);

				if (_playerData._player->GetVx() < -Define::PLAYER_MAX_RUNNING_SPEED)
					_playerData._player->SetVx(-Define::PLAYER_MAX_RUNNING_SPEED);
			}
		}
	}
	else
		_playerData._player->setState(new PlayerStandingState(_playerData));
}

void PlayerRunningState::onCollide(Entity* impactor, Entity::eSide side, Entity::CollisionReturn data)
{
	switch (side)
	{
	case Entity::Left:
	{
		if (_playerData._player->getMoveDirection() == Player::MoveToLeft)
		{
			_playerData._player->setAllowMoveLeft(false);
			_playerData._player->AddPosition(data._RegionCollision.right - data._RegionCollision.left, 0);
			_playerData._player->setState(new PlayerStandingState(_playerData));
		}

		break;
	}

	case Entity::Right:
	{
		if (_playerData._player->getMoveDirection() == Player::MoveToRight)
		{
			_playerData._player->setAllowMoveRight(false);
			_playerData._player->AddPosition(-(data._RegionCollision.right - data._RegionCollision.left), 0);
			_playerData._player->setState(new PlayerStandingState(_playerData));
		}
		break;
	}

	case Entity::Bottom:
	{
		_playerData._player->AddPosition(0, -(data._RegionCollision.bottom - data._RegionCollision.top));
		_playerData._player->SetVy(0);
		break;
	}
	default: break;
	}
}

PlayerState::StateName PlayerRunningState::GetState()
{
	return PlayerState::Running;
}

void PlayerRunningState::onCollision(Entity::eSide side)
{
	switch (side) {

	case Entity::Left:
	case Entity::Right:
		_playerData._player->SetVx(0);
		_playerData._player->setState(new PlayerStandingState(_playerData));
		break;

	default: break;
	}
}
