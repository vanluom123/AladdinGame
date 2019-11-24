#pragma once

#include "../../../GameComponents/Animation.h"
#include "../../../GameComponents/GameGlobal.h"
#include "../../../GameObjects/Entity.h"
#include "../../../GameComponents/GameCollision.h"
#include "CivilianEnemy1stState.h"
#include "CivilianEnemy1stData.h"
#include "CivilianEnemy1stThrowState.h"
#include "../../Player/Player.h"

class CivilianEnemy1st :public Entity
{
public:
	CivilianEnemy1st();
	CivilianEnemy1st(GVector2 position, Player* mplayer);
	~CivilianEnemy1st();

	Player* mPlayer;

	void Update(float dt);
	void SetState(CivilianEnemy1stState* newState);
	virtual CivilianEnemy1stState::StateName CivilianEnemy1st::GetState();
	void Draw(GVector3 position = GVector3(), RECT sourceRect = RECT(), GVector2 scale = GVector2(), GVector2 transform = GVector2(), float angle = 0, GVector2 rotationCenter = GVector2(), D3DXCOLOR colorKey = D3DCOLOR_XRGB(71, 33, 222));
	virtual void Draw(GVector2 transform);
	void OnCollision(Entity* impactor, Entity::CollisionReturn data, Entity::SideCollisions side);
	virtual RECT GetBound();
	Animation* GetCurrentAnimation();
	Animation* GetAnimation(CivilianEnemy1stState::StateName state);
	bool IsNearToPlayer();

protected:
	void OnSetPosition(GVector3 poks);
	void changeAnimation(CivilianEnemy1stState::StateName state);

	CivilianEnemy1stData* mCivilianEnemy1stData;
	Animation* mCurrentAnimation;
	Animation* mAnimationThrow;
	CivilianEnemy1stState::StateName mCurrentState;
	bool isNear = false;
};
