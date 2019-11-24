#pragma once

#include "../../GameComponents/Animation.h"
#include "../../GameComponents/GameGlobal.h"
#include "../../GameObjects/Entity.h"
#include "../../GameComponents/GameCollision.h"
#include "JafarState.h"
#include "JafarData.h"
#include "JafarOriginalState.h"
#include "JafarSnakeState.h"
#include "../../GameObjects/Player/Player.h"
#include "../../Scenes/EndScene.h"

class Jafar :public Entity
{
public:
	Jafar();
	Jafar(GVector2 position, Player* player);
	~Jafar();

	Player* mPlayer;

	void Update(float dt);
	void SetState(JafarState* newState);
	virtual JafarState::StateName Jafar::GetState();
	void Draw(GVector3 position = GVector3(), RECT sourceRect = RECT(), GVector2 scale = GVector2(), GVector2 transform = GVector2(), float angle = 0, GVector2 rotationCenter = GVector2(), D3DXCOLOR colorKey = D3DCOLOR_XRGB(255, 255, 255));
	void Draw(GVector2 transfrom);
	RECT GetBound() override;
	void OnCollision(Entity* impactor, CollisionReturn data, SideCollisions side);
	Animation* GetCurrentAnimation();
	Animation* GetAnimation(JafarState::StateName state);
	void SetReverse(bool flag);
	bool GetReverse();
	bool IsInActiveRegion();

protected:
	void changeAnimation(JafarState::StateName state);

	bool mCurrentReverse = false;
	JafarData* mJafarData;
	Animation* mCurrentAnimation,
		* mAnimationOriginal,
		* mAnimationSnake;
	JafarState::StateName mCurrentState;
};