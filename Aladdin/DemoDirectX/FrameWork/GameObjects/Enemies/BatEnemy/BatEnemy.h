#pragma once
#include "../../../GameComponents/Animation.h"
#include "../../../GameComponents/GameGlobal.h"
#include "../../../GameObjects/Entity.h"
#include "../../../GameComponents/GameCollision.h"
#include "BatEnemyData.h"
#include "BatEnemyState.h"
#include "BatEnemyFlyState.h"
#include "../../Player/Player.h"

class BatEnemy :public Entity
{
public:
	BatEnemy();
	BatEnemy(GVector2 position, Player* mplayer);
	~BatEnemy();

	Player* mPlayer;

	void Update(float dt);
	void Draw(GVector3 position = GVector3(), RECT sourceRect = RECT(), GVector2 scale = GVector2(), GVector2 transform = GVector2(), float angle = 0, GVector2 rotationCenter = GVector2(), D3DXCOLOR colorKey = D3DCOLOR_XRGB(71, 33, 222));
	virtual void Draw(GVector2 transform);
	void OnCollision(Entity* impactor, Entity::CollisionReturn data, Entity::SideCollisions side);
	void SetState(BatEnemyState* newState);

	RECT GetBound() override;

	Animation* GetCurrentAnimation();
	Animation* GetAnimation(BatEnemyState::StateName state);

	bool IsNearToPlayer();

protected:
	void OnSetPosition(GVector3 pos);
	void changeAnimation(BatEnemyState::StateName state);

	BatEnemyData* mBatEnemyData;
	Animation* mCurrentAnimation;
	Animation* mAnimationFly;
	BatEnemyState::StateName mCurrentState;
	bool isNear = false;
};

