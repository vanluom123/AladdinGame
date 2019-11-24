#pragma once

#include "../../GameComponents/Animation.h"
#include "../Entity.h"
#include "../../GameComponents/GameMap.h"


class Pendulum : public Entity
{
public:
	~Pendulum();
	Pendulum();
	Pendulum(GVector3 position);
	Pendulum(GVector2 position);
	void Update(float dt);
	RECT GetBound();
	void Draw(GVector3 position = GVector3(), RECT sourceRect = RECT(), GVector2 scale = GVector2(), GVector2 transform = GVector2(), float angle = 0, GVector2 rotationCenter = GVector2(), D3DXCOLOR colorKey = D3DCOLOR_XRGB(255, 255, 255));
	void Draw(GVector3 transform);
	void Draw(GVector2 transform);
	void OnCollision(Entity* impactor, Entity::CollisionReturn data, Entity::SideCollisions side) override;

protected:
	bool init(GVector3 position);
	void OnSetPosition(GVector3 poks);

	Animation* mAnimation;
	bool isCollision_with_Aladdin;
};
