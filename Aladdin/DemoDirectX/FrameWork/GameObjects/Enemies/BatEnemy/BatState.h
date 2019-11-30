#pragma once
#include "../../Entity.h"
#include "BatData.h"

class BatState
{
public:
	BatState(BATDATA batEnemyData) {
		mBatEnemyData = batEnemyData;
	}
	BatState() = default;
	~BatState() = default;

	enum StateName
	{
		eBat_None,
		fly
	};

	virtual void Update(float dt) {}
	virtual void OnCollision(Entity* impactor, Entity::SideCollisions side, Entity::CollisionReturn data) {}
	virtual StateName GetState() = 0;

protected:
	BATDATA mBatEnemyData;
};

