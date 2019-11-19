#pragma once
#include "PlayerState.h"
#include "Player.h"

class PlayerRunningState : public PlayerState
{
protected:
	float _acceleratorX;
	static const float SPEED;

public:
    PlayerRunningState(PlayerData playerData);
    ~PlayerRunningState();

    void keyboardEventHandler(std::map<int, bool> keys) override;
    void onCollide(Entity *impactor, Entity::eSide side, Entity::CollisionReturn data);
    virtual StateName GetState();

	void onCollision(Entity::eSide side) override;
};