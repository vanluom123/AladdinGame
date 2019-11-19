#include "PlayerFallingState.h"
#include "Player.h"
#include "PlayerStandingState.h"
#include "../../GameComponents/GameCollision.h"
#include "../../GameDefines/GameDefine.h"

PlayerFallingState::PlayerFallingState(PlayerData playerData)
{
	_playerData = playerData;

	_acceleratorY = 15.0f;
	_acceleratorX = 8.0f;

	_isLeftOrRightKeyPressed = false;

	if (_playerData._player->GetVx() == 0)
		_allowMoveX = false;
	else
		_allowMoveX = true;
}


PlayerFallingState::~PlayerFallingState()
{}

void PlayerFallingState::update(float dt)
{
	_playerData._player->AddVy(_acceleratorY);

	if (_playerData._player->GetVy() > Define::PLAYER_MAX_JUMP_VELOCITY)
		_playerData._player->SetVy(Define::PLAYER_MAX_JUMP_VELOCITY);
}

void PlayerFallingState::keyboardEventHandler(std::map<int, bool> keys)
{
	if (keys[VK_RIGHT])
	{
		_playerData._player->Reverse = false;

		_isLeftOrRightKeyPressed = true;
		if (_playerData._player->GetVx() < Define::PLAYER_MAX_RUNNING_SPEED)
		{
			_playerData._player->AddVx(_acceleratorX);

			if (_playerData._player->GetVx() >= Define::PLAYER_MAX_RUNNING_SPEED)
				_playerData._player->SetVx(Define::PLAYER_MAX_RUNNING_SPEED);
		}
	}
	else if (keys[VK_LEFT])
	{
		_playerData._player->Reverse = true;

		_isLeftOrRightKeyPressed = true;
		if (_playerData._player->GetVx() > -Define::PLAYER_MAX_RUNNING_SPEED)
		{
			_playerData._player->AddVx(-_acceleratorX);

			if (_playerData._player->GetVx() <= -Define::PLAYER_MAX_RUNNING_SPEED)
				_playerData._player->SetVx(-Define::PLAYER_MAX_RUNNING_SPEED);
		}
	}
	else
		_isLeftOrRightKeyPressed = false;
}

void PlayerFallingState::onCollide(Entity* impactor, Entity::eSide side, Entity::CollisionReturn data)
{
	switch (side)
	{
		case Entity::Left:
			if (_playerData._player->getMoveDirection() == Player::MoveToLeft)
			{
				_playerData._player->AddPosition(data._RegionCollision.right - data._RegionCollision.left, 0);
				_playerData._player->SetVx(0);
			}
			break;

		case Entity::Right:
			if (_playerData._player->getMoveDirection() == Player::MoveToRight)
			{
				_playerData._player->AddPosition(-(data._RegionCollision.right - data._RegionCollision.left), 0);
				_playerData._player->SetVx(0);
			}
			break;

		case Entity::Top:
			break;

		case Entity::Bottom:
			if (data._RegionCollision.right - data._RegionCollision.left >= 8.0f)
			{
				_playerData._player->AddPosition(0, -(data._RegionCollision.bottom - data._RegionCollision.top));

				if (_isLeftOrRightKeyPressed)
					_playerData._player->setState(new PlayerRunningState(_playerData));
				else
					_playerData._player->setState(new PlayerStandingState(_playerData));
			}
			return;

		default:
			break;
	}
}

PlayerState::StateName PlayerFallingState::GetState()
{
	return PlayerState::Falling;
}

void PlayerFallingState::onCollision(Entity::eSide side)
{
	switch (side) {

	case Entity::Left:
	case Entity::Right:
		_playerData._player->SetVx(0.0f);
		break;

	case Entity::Bottom:
		if (_isLeftOrRightKeyPressed)
			_playerData._player->setState(new PlayerRunningState(_playerData));
		else
			_playerData._player->setState(new PlayerStandingState(_playerData));
		break;

	default: break;
	}
}
