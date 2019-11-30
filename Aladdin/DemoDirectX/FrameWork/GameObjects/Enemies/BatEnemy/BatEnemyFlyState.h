#pragma once
#include "Bat.h"
#include "BatState.h"

class BatEnemyFlyState: public BatState
{
public:
	BatEnemyFlyState(BATDATA batEnemyData);
	~BatEnemyFlyState() = default;

	void Update(float dt) override;
	BatState::StateName GetState();

private:
	bool isFly = false;
};

