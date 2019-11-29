#ifndef __SKETELON_H__
#define __SKETELON_H__
#include "../../Entity.h"
#include "../../../GameComponents/Animation.h"

class Sketelon : public Entity
{

	Animation* _anim;

public:
	Sketelon(GVector2 position);
	~Sketelon();

	void initialize(GVector2 position);

	void Draw(GVector2 trans) override;
	void Update(float dt) override;
	void OnCollision(Entity* impactor, CollisionReturn data, SideCollisions side) override;
	RECT GetBound() override;

};

#endif // __SKETELON_H__
