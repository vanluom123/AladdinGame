#pragma once

#include "../../../GameComponents/Animation.h"
#include "../../../GameComponents/GameGlobal.h"
#include "../../../GameObjects/Entity.h"
#include "../../../GameComponents/GameCollision.h"
#include "../../Player/Player.h"
#include "../Guard_3rd/Guard3State.h"

class Guard3 :public Entity
{
public:
	Guard3();
	Guard3(GVector2 position, Player* player);
	~Guard3();

	Player* mPlayer;

	void Update(float dt);
	void SetState(Guard3State* newState);
	virtual Guard3State::StateName Guard3::GetState();
	void Draw(GVector3 position = GVector3(), RECT sourceRect = RECT(), GVector2 scale = GVector2(), GVector2 transform = GVector2(), float angle = 0, GVector2 rotationCenter = GVector2(), D3DXCOLOR colorKey = D3DCOLOR_XRGB(255, 255, 255));
	void Draw(GVector2 transfrom);
	RECT GetBound() override;
	void OnCollision(Entity* impactor, Entity::CollisionReturn data, Entity::SideCollisions side);
	Animation* GetCurrentAnimation();
	Animation* GetAnimation(Guard3State::StateName state);
	void SetReverse(bool flag);
	bool GetReverse();
	bool IsNearToPlayer();
	bool IsInAttackRange();
	bool IsInActiveRegion();
	void LimitDistance();

	int minX, maxX;


protected:
	void changeAnimation(Guard3State::StateName state);

	int x;	// Toa do x luc khoi tao 
			// khoang gioi han di chuyen cua enemy
	bool mCurrentReverse = false;

	Guard3Data* mGuard3Data;
	Animation* mCurrentAnimation,
		* mAnimationDance,
		* mAnimationBeaten,
		* mAnimationRun,
		* mAnimationStand,
		* mAnimationThrow;
	Guard3State::StateName mCurrentState;
};