#pragma once
#include "Player.h"
#include "PlayerState.h"

class PlayerJumpingState : public PlayerState
{
protected:
	float _acceleratorY;
	float _acceleratorX;
	bool _noPressed;
	bool _allowMoveRight, _allowMoveLeft;

public:
    PlayerJumpingState(PlayerData playerData);
    ~PlayerJumpingState();

    void update(float dt);
    void keyboardEventHandler(std::map<int, bool> keys);
    void onCollide(Entity *impactor, Entity::eSide side, Entity::CollisionReturn data);
    virtual StateName GetState();

	void onCollision(Entity::eSide side) override;
};

