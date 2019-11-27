#include "IComponent.h"

Movement::Movement(GVector2 accel, GVector2 vec, Sprite* refSprite)
{
	_accel = accel;
	_velocity = vec;
	_refSprite = refSprite;
}

void Movement::update(float deltaTime)
{
	if (_refSprite == NULL)
		return;
	auto position = _refSprite->GetPosition();
	_velocity += _accel * deltaTime / 1000;
	position.x += _velocity.x * deltaTime / 1000;
	position.y += _velocity.y * deltaTime / 1000;
}

Gravity::Gravity(GVector2 gravity, Movement* movement)
{
	this->_gravity = gravity;
	this->_refmovement = movement;
	this->_status = FALLING__DOWN;
}

void Gravity::update(float deltaTime)
{
	switch (_status)
	{
	case FALLING__DOWN:
		this->_additionalVeloc = this->_gravity * deltaTime / 1000;
		break;
	case SHALLOWED:
		this->_additionalVeloc = GVector2(0, 0);
	default:
		break;
	}
	auto veloc = this->_refmovement->GetVelocity();
	this->_refmovement->SetVelocity(veloc + _additionalVeloc);
}

RoundMovement::RoundMovement(float radius, float frequency, float radian, Sprite* refSprite)
{
	_radius = radius;
	_radianVeloc = frequency;
	this->SetAngularVeloc(_radianVeloc);
	this->_refSprite = refSprite;
	_radian = radian;
	_roundposition = GVector2(_radius * cos(_radian), _radius * sin(_radian));
}

RoundMovement::~RoundMovement()
{
}

void RoundMovement::update(float deltaTime)
{
	/*
	góc xoay được tính theo công thức
	φ = ω * t
	*/

	_radian += _radianVeloc * deltaTime / 1000;
	/*
	vận tốc tuyến tính được tính theo công thức
			x = R * cos(φ)
	và		y = R * sin(φ)
	*/
	GVector2 veloc;
	veloc.x = _radius * sin(_radian);
	veloc.y = -_radius * cos(_radian);

	auto pos = this->_refSprite->GetPosition();
	pos.x += veloc.x * deltaTime / 1000;
	pos.y += veloc.y * deltaTime / 1000;
	this->_refSprite->SetPosition(pos);
}
