#pragma once

#include "../../../GameComponents/Animation.h"
#include "../../../GameComponents/GameGlobal.h"
#include "../../../GameObjects/Entity.h"
#include "../../../GameComponents/GameCollision.h"
#include "CivilianEnemy2ndState.h"
#include "CivilianEnemy2ndData.h"
#include "CivilianEnemy2ndThrowState.h"
#include "../../Player/Player.h"


class CivilianEnemy2nd :public Entity
{
public:

	CivilianEnemy2nd();
	CivilianEnemy2nd(GVector2 position, Player* mplayer);
	~CivilianEnemy2nd();

	Player* mPlayer;

	void Update(float dt);
	void SetState(CivilianEnemy2ndState* newState);
	virtual CivilianEnemy2ndState::StateName CivilianEnemy2nd::GetState();
	void Draw(GVector3 position = GVector3(), RECT sourceRect = RECT(), GVector2 scale = GVector2(), GVector2 transform = GVector2(), float angle = 0, GVector2 rotationCenter = GVector2(), D3DXCOLOR colorKey = D3DCOLOR_XRGB(71, 33, 222));
	virtual void Draw(GVector2 transform);
	virtual RECT GetBound();
	void OnCollision(Entity* impactor, Entity::CollisionReturn data, Entity::SideCollisions side);
	Animation* GetCurrentAnimation();
	Animation* GetAnimation(CivilianEnemy2ndState::StateName state);
	void SetReverse(bool flag);
	bool IsNearToPlayer();

protected:
	void OnSetPosition(GVector3 poks);
	void changeAnimation(CivilianEnemy2ndState::StateName state);

	CivilianEnemy2ndData* mCivilianEnemy2ndData;
	Animation* mCurrentAnimation,
		* mAnimationThrow;
	CivilianEnemy2ndState::StateName mCurrentState;
	bool mCurrentReverse = false;
};
