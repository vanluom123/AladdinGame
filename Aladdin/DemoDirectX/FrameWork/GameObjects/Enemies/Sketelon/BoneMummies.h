#ifndef __BONE2_H__
#define __BONE2_H__
#include "../../Entity.h"
#include "../../../GameComponents/Animation.h"

class BoneMummies : public Entity
{
	Animation* _anim;

public:
	BoneMummies(GVector2 position);
	~BoneMummies();

	void initialize(GVector2 position);

	void Draw(GVector2 trans) override;
	RECT GetBound() override;
	void Update(float dt) override;
	void OnCollision(Entity* impactor, CollisionReturn data, SideCollisions side) override;
};

#endif // __BONE2_H__
