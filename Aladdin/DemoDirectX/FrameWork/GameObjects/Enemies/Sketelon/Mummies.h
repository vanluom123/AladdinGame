#ifndef __MUMMIES_H__
#define __MUMMIES_H__

#include "../../Entity.h"
#include "../../../GameComponents/Animation.h"
class Mummies :	public Entity
{
	Animation* _anim;

public:
	Mummies(GVector2 position);
	Mummies() = default;
	~Mummies();

	void initialize(GVector2 position);

	void Draw(GVector2 trans) override;
	void Update(float dt) override;
	void OnCollision(Entity* impactor, CollisionReturn data, SideCollisions side) override;
	RECT GetBound() override;

};

#endif
