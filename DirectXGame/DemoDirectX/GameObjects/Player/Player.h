#pragma once
#include "../../GameComponents/Animation.h"
#include "../../GameComponents/GameGlobal.h"
#include "../../GameComponents/Viewport.h"
#include "PlayerData.h"
#include "PlayerState.h"
#include "PlayerRunningState.h"
#include "../../GameComponents/GameDebugDraw.h"

class Player : public Entity
{

protected:

	// Viewport pointer
	Viewport* _pViewport;

	// Middle
	PlayerData _playerData;

	// To draw rectangle
	GameDebugDraw* _pGameDebugDraw;

	// Current status of player
	PlayerState::StateName _currentState;

	// the animation of player
	Animation* _currentAnim;

	bool _allowJump;
	bool _allowMoveLeft;
	bool _allowMoveRight;

public:
	Player();
	~Player();

	// Moving direction of player
	enum MoveDirection
	{
		MoveToLeft,
		MoveToRight,
		None
	};

	// Update player
	void update(float dt) override;

	// Draw player
	void draw(D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));

	// Player resolve collision
	void OnCollision(Entity* impactor, Entity::CollisionReturn data, Entity::eSide side);
	void onNoCollisionWithBottom();

	// Update Player's status
	void setState(PlayerState* newState);
	PlayerState::StateName Player::getState();

	// Resolve keyboard
	void keyboardEventHandler(std::map<int, bool> keys);
	void onKeyPressed(int key);
	void onKeyUp(int key);

	// flag = true -> flip image to horizontal
	// flag = false -> don't flip image
	void setFlipHorizontal(bool flag);

	// Setting viewport
	void setCamera(Viewport* camera);

	// Get moving direction
	MoveDirection getMoveDirection();

	// Get bounding
	RECT GetBounding() override;

	// Allowing to move left
	bool allowMoveLeft() const;
	void setAllowMoveLeft(bool val);

	// Allowing to move right
	bool allowMoveRight() const;
	void setAllowMoveRight(bool val);

	// Collision handle
	void OnCollision(eSide side) override;

	// PROPERTY
	// Setting FlipHorizontal
	SET(Player, bool, Reverse, { _currentAnim->setFlipHorizontal(value); })

	// Setting viewport
	SET(Player, Viewport*, setViewport, { _pViewport = value; })

};

