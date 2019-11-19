#include "Player.h"
#include "PlayerFallingState.h"
#include "PlayerJumpingState.h"
#include "../../GameComponents/GameCollision.h"
#include "../../GameComponents/GameLog.h"

Player::Player() : Reverse(this), setViewport(this)
{
	_pGameDebugDraw = new GameDebugDraw();

	_currentAnim = new Animation(eID::PLAYER, "sketelon", 20, 0.05);

	_playerData = PlayerData();
	_playerData._player = this;
	this->setState(new PlayerFallingState(_playerData));

	_allowJump = true;

	this->Reverse = false;
}

Player::~Player()
{
	delete _pViewport;
	delete _playerData._pState;
	delete _currentAnim;
}

void Player::update(float dt)
{
	// check collision with bottom
	this->onNoCollisionWithBottom();

	// Update animation
	_currentAnim->update(dt);

	// Update entity
	Entity::update(dt);

	// Update status of player
	if (_playerData._pState)
		_playerData._pState->update(dt);
}

void Player::keyboardEventHandler(std::map<int, bool> keys)
{
	if (_playerData._pState)
		_playerData._pState->keyboardEventHandler(keys);
}

void Player::onKeyPressed(int key)
{
	if (key == VK_SPACE)
	{
		if (_allowJump)
		{
			if (_currentState == PlayerState::Running || _currentState == PlayerState::Standing)
				setState(new PlayerJumpingState(_playerData));

			_allowJump = false;
		}
	}
}

void Player::onKeyUp(int key)
{
	if (key == VK_SPACE)
		_allowJump = true;
}

void Player::setFlipHorizontal(bool flag)
{
	_currentAnim->setFlipHorizontal(flag);
}

void Player::setCamera(Viewport* camera)
{
	_pViewport = camera;
}

void Player::draw(D3DCOLOR color)
{
	if (_pViewport)
	{
		_pGameDebugDraw->DrawRect(this->GetBounding(), _pViewport);
		_currentAnim->draw(this->getPosition(), RECT(), GVector2(), _pViewport->getTranslate(), 0, GVector2(), color);
	}
	else
		_currentAnim->draw(this->getPosition());
}

void Player::setState(PlayerState* newState)
{
	delete _playerData._pState;
	_playerData._pState = newState;
	_currentState = newState->GetState();
}

void Player::OnCollision(Entity* impactor, Entity::CollisionReturn data, Entity::eSide side)
{
	if (_playerData._pState)
		_playerData._pState->onCollide(impactor, side, data);
}

void Player::OnCollision(eSide side)
{
	if (_playerData._pState)
		_playerData._pState->onCollision(side);
}

RECT Player::GetBounding()
{
	RECT r;
	r.left = _X - _currentAnim->getAnchorPoint().x;
	r.top = _Y - _currentAnim->getAnchorPoint().y;
	r.right = r.left + _currentAnim->getSprite()->getWidth() - 1;
	r.bottom = r.top + _currentAnim->getSprite()->getHeight() - 1;
	return r;
}

bool Player::allowMoveLeft() const { return _allowMoveLeft; }

void Player::setAllowMoveLeft(bool val)
{
	if (val != _allowMoveLeft)
		_allowMoveLeft = val;
}

bool Player::allowMoveRight() const { return _allowMoveRight; }

void Player::setAllowMoveRight(bool val)
{
	if (val != _allowMoveRight)
		_allowMoveRight = val;
}

Player::MoveDirection Player::getMoveDirection()
{
	if (_Vx > 0)
		return MoveDirection::MoveToRight;

	else if (_Vx < 0)
		return MoveDirection::MoveToLeft;

	return MoveDirection::None;
}

void Player::onNoCollisionWithBottom()
{
	if (_SideY != Entity::Bottom)
	{
		if (_currentState != PlayerState::Jumping && _currentState != PlayerState::Falling)
			this->setState(new PlayerFallingState(_playerData));
	}
}

PlayerState::StateName Player::getState()
{
	return _currentState;
}