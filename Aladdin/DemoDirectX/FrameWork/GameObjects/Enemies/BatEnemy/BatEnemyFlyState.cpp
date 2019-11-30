#include "BatEnemyFlyState.h"

BatEnemyFlyState::BatEnemyFlyState(BATDATA batEnemyData)
{
	mBatEnemyData = batEnemyData;
}

void BatEnemyFlyState::Update(float dt)
{
	if (mBatEnemyData.batEnemy->GetCurrentAnimation()->mCurrentIndex == 8 && isFly == true)
	{
		if (mBatEnemyData.batEnemy->getFaceDirection() == Entity::LEFT)
		{
		}
	}
}


BatState::StateName BatEnemyFlyState::GetState()
{
	return BatState::fly;
}
