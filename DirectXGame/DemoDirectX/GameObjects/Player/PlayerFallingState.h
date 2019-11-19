#pragma once
#include "PlayerState.h"

class PlayerFallingState : public PlayerState
{
protected:
	float _acceleratorY;
	float _acceleratorX;

	bool _allowMoveX;

	bool _isLeftOrRightKeyPressed;

public:
    PlayerFallingState(PlayerData playerData);
    ~PlayerFallingState();

    void update(float dt);
    void keyboardEventHandler(std::map<int, bool> keys) override;
    void onCollide(Entity *impactor, Entity::eSide side, Entity::CollisionReturn data);
    virtual StateName GetState();

	void onCollision(Entity::eSide side) override;
};