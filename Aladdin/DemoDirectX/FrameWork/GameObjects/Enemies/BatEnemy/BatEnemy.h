#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "../../../GameComponents/Animation.h"
#include "../../../GameComponents/GameGlobal.h"
#include "../../../GameObjects/Entity.h"
#include "../../../GameComponents/GameCollision.h"
#include "BatEnemyData.h"
#include "BatEnemyState.h"
#include "BatEnemyFlyState.h"
#include "../../Player/Player.h"

class GameMap;
class BatEnemy :public Entity
{
public:
	BatEnemy();
	BatEnemy(D3DXVECTOR2 position, Player* mplayer);
	~BatEnemy();

	Player *mPlayer;

	void SetGameMap(GameMap *gamemap);

	GameMap* getGameMap();

	void Update(float dt);

	void SetState(BatEnemyState *newState);



	void Draw(D3DXVECTOR3 position = D3DXVECTOR3(), RECT sourceRect = RECT(), D3DXVECTOR2 scale = D3DXVECTOR2(), D3DXVECTOR2 transform = D3DXVECTOR2(), float angle = 0, D3DXVECTOR2 rotationCenter = D3DXVECTOR2(), D3DXCOLOR colorKey = D3DCOLOR_XRGB(71, 33, 222));
	virtual void Draw(D3DXVECTOR2 transform);
	void OnCollision(Entity *impactor, Entity::CollisionReturn data, Entity::SideCollisions side);

	virtual RECT GetBound();

	Animation *GetCurrentAnimation();

	Animation *GetAnimation(BatEnemyState::StateName state);

	bool IsNearToPlayer();
protected:
	void OnSetPosition(D3DXVECTOR3 pos);

	BatEnemyData *mBatEnemyData;

	GameMap *mGamMap;

	Animation *mCurrentAnimation,
			   *mAnimationFly;

	void changeAnimation(BatEnemyState::StateName state);

	BatEnemyState::StateName mCurrentState;

	bool isNear = false;
};

