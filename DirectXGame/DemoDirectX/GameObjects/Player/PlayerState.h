#pragma once
#include "PlayerData.h"
#include "../Entity.h"
#include "../../GameComponents/GameCollision.h"
#include "../../GameComponents/Animation.h"


class PlayerState
{
protected:
	PlayerData _playerData;
	Animation* _currentAnim;

public:
	enum StateName
	{
		Standing,
		Running,
		Falling,
		Jumping,
		Die
	};

	PlayerState(PlayerData playerData) {
		_playerData = playerData;
		_currentAnim = NULL;
	}

	PlayerState() {
		_playerData = PlayerData();
		_currentAnim = NULL;
	}

	~PlayerState() {
	}

	virtual void update(float dt) {}
	virtual void keyboardEventHandler(std::map<int, bool> keys) {}
	virtual void onCollide(Entity* impactor, Entity::eSide side, Entity::CollisionReturn data) {}
	virtual StateName GetState() = 0;
	virtual void onCollision(Entity::eSide side) {}
};