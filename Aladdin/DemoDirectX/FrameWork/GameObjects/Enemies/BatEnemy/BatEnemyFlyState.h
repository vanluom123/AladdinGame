#pragma once
#include "BatEnemy.h"
#include "BatEnemyState.h"

class BatEnemyFlyState: public BatEnemyState
{
public:
	BatEnemyFlyState(BatEnemyData *batEnemyData);
	~BatEnemyFlyState();

	void Update(float dt);

	BatEnemyState::StateName GetState();

private:
	bool isFly = false;
};

