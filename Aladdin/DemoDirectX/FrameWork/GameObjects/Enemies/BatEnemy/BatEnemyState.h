#pragma once
#include "../../Entity.h"
#include "BatEnemyData.h"

class BatEnemyState
{
public:
	
	BatEnemyState();
	~BatEnemyState();
	enum StateName
	{
		fly
	};
	virtual void Update(float dt);
	virtual void OnCollision(Entity *impactor, Entity::SideCollisions side, Entity::CollisionReturn data);

	virtual StateName GetState() = 0;

protected:

	BatEnemyState(BatEnemyData *batEnemyData);

	BatEnemyData *mBatEnemyData;
};

