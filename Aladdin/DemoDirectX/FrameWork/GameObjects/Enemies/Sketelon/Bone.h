#ifndef __BONE_H__
#define __BONE_H__
#include "../../Entity.h"
#include "../../../GameComponents/Animation.h"

class Bone : public Entity
{

protected:
	Animation* _anim;

public:
	Bone();
	Bone(GVector2 position);
	~Bone();

	void initialize(GVector2 position);

	void Draw(D3DXVECTOR2 trans) override;
	void Update(float dt) override;
	RECT GetBound() override;
	void OnCollision(Entity* impactor, CollisionReturn data, SideCollisions side) override;
};

#endif // __BONE_H__
