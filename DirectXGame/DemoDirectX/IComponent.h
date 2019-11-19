#ifndef __ICOMPONENT_H__
#define __ICOMPONENT_H__

#include "GameDefines/GameDefine.h"
#include "GameComponents/Sprite.h"
#include "Utils/properties.h"
#include <corecrt_math_defines.h>

class IComponent
{
public:
	virtual void update(float deltaTime) = 0;
};

class Movement : public IComponent
{
	GVector2 _accel;
	GVector2 _velocity;
	Sprite* _refSprite;

public:
	Movement(GVector2 accel, GVector2 vec, Sprite* refSprite);
	void update(float deltaTime) override;

	GETSET(Movement, GVector2, Accelerate, { return _accel; }, { _accel = value; })
		GETSET(Movement, GVector2, Velocity, { return _velocity; }, { _velocity = value; })
};

enum eGravityStatus
{
	FALLING__DOWN,
	SHALLOWED
};

class Gravity : public IComponent
{
	GVector2 _gravity;
	GVector2 _additionalVeloc;
	Movement* _refmovement;
	eGravityStatus _status;
	Sprite* _refSprite;
public:
	Gravity(GVector2 gravity, Movement* movement);
	void update(float deltaTime) override;

	SET(Gravity, GVector2, SetGravity, { _gravity = value; })
		SET(Gravity, eGravityStatus, Status, { _status = value; })
};

class RoundMovement : public IComponent
{
	Sprite* _refSprite;
	float _radius;				// Bán Kính.
	GVector2 _roundposition;
	float _radianVeloc;			// vận tốc góc (= ω)
	float _radian;				// góc xoay (= φ) biến thiên theo thời gian
public:
	RoundMovement(float radius, float frequency, float radian, Sprite* refSprite);
	~RoundMovement();
	void update(float deltaTime) override;

	GETSET(RoundMovement, float, AngularVeloc, { return _radianVeloc; }, {
		_radianVeloc = value * 2 * M_PI;
		})

	GETSET(RoundMovement, float, Radius, { return _radius; }, { _radius = value; })
};

#endif // _DEBUG
