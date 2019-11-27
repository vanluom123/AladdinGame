#pragma once

#include "../../GameComponents/Animation.h"
#include "../Entity.h"
#include "../../GameComponents/GameMap.h"


class Pendulum : public Entity
{
public:
	~Pendulum();
	Pendulum(GVector2 position);

	void Update(float dt);
	RECT GetBound() override;
	void Draw(GVector3 position = GVector3(), RECT sourceRect = RECT(), GVector2 scale = GVector2(), GVector2 transform = GVector2(), float angle = 0, GVector2 rotationCenter = GVector2(), D3DXCOLOR colorKey = D3DCOLOR_XRGB(255, 255, 255));
	void Draw(GVector2 transform);
	void OnCollision(Entity* impactor, CollisionReturn data, SideCollisions side) override;

	void SetStatus(const char* val)
	{
		_status = atoi(val);
		if (_status == 0)
			_anim->mCurrentIndex = 1;
		else if (_status == 1)
			_anim->mCurrentIndex = 15;
	}



protected:
	void init(GVector2 position);
	Animation* _anim;

	float _time;

	// 1: open
	// 0: close
	int _status;
};
