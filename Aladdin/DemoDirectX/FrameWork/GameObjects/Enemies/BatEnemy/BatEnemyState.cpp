#include "BatEnemyState.h"

void BatEnemyState::Update(float dt)
{
}

void BatEnemyState::OnCollision(Entity * impactor, Entity::SideCollisions side, Entity::CollisionReturn data)
{
}

BatEnemyState::BatEnemyState()
{
}

BatEnemyState::~BatEnemyState()
{
}

BatEnemyState::BatEnemyState(BatEnemyData *batEnemyData)
{
	this->mBatEnemyData = batEnemyData;
}


