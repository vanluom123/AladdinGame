#include "BatEnemyFlyState.h"

BatEnemyFlyState::BatEnemyFlyState(BatEnemyData * batEnemyData)
{
	this->mBatEnemyData = batEnemyData;
}

BatEnemyFlyState::~BatEnemyFlyState()
{
}

void BatEnemyFlyState::Update(float dt)
{
	if (this->mBatEnemyData->batEnemy->GetCurrentAnimation()->mCurrentIndex == 8 && isFly == true)
	{
		Entity* tmp = nullptr;
		if (this->mBatEnemyData->batEnemy->getFaceDirection() == Entity::LEFT)
		{
			
		}
	}
}

  
BatEnemyState::StateName BatEnemyFlyState::GetState()
{
	return BatEnemyState::fly;
}
