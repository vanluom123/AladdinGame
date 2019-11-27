#ifndef __ICOMPONENT_H__
#define __ICOMPONENT_H__

#include <corecrt_math_defines.h>
#include "../GameDefines/define.h"
#include "Sprite.h"

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

	GVector2 GetVelocity() { return _velocity; }
	void SetVelocity(GVector2 val) { _velocity = val; }

	GVector2 GetAccelerate() { return _accel; }
	void SetAccelerate(GVector2 val) { _accel = val; }
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

	void SetGravity(GVector2 val) { _gravity = val; }
	void SetGravityStatus(eGravityStatus val) { _status = val; }
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

	float GetAngularVeloc() { return _radianVeloc; }
	void SetAngularVeloc(float val) {
		_radianVeloc = val * 2 * M_PI;
	}

	float GetRadius() { return _radius; }
	void SetRadius(float val) { _radius = val; }
};

#endif // _DEBUG
