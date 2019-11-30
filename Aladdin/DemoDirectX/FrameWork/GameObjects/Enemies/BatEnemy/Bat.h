#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "../../../GameComponents/Animation.h"
#include "../../../GameComponents/GameGlobal.h"
#include "../../../GameObjects/Entity.h"
#include "../../../GameComponents/GameCollision.h"
#include "BatData.h"
#include "BatState.h"
#include "BatEnemyFlyState.h"
#include "../../Player/Player.h"

class GameMap;
class Bat :public Entity
{
public:
	Bat();
	Bat(D3DXVECTOR2 position, Player* mplayer);
	~Bat();

	Player* m_pPlayer;

	void Update(float dt);
	void Draw(D3DXVECTOR3 position = D3DXVECTOR3(), RECT sourceRect = RECT(), D3DXVECTOR2 scale = D3DXVECTOR2(), D3DXVECTOR2 transform = D3DXVECTOR2(), float angle = 0, D3DXVECTOR2 rotationCenter = D3DXVECTOR2(), D3DXCOLOR colorKey = D3DCOLOR_XRGB(71, 33, 222));
	void Draw(D3DXVECTOR2 transform);
	void OnCollision(Entity* impactor, Entity::CollisionReturn data, Entity::SideCollisions side);
	RECT GetBound();

	Animation* GetCurrentAnimation();
	Animation* GetAnimation(BatState::StateName state);

	bool IsNearToPlayer();

	void SetGameMap(GameMap* gamemap);
	GameMap* getGameMap();

	void ChangeState(BatState* newState);


protected:
	void OnSetPosition(D3DXVECTOR3 pos);
	void changeAnimation(BatState::StateName state);

	BATDATA m_data;
	GameMap* m_pGamMap;
	Animation* m_CurrentAnimation;
	Animation* m_animationFly;
	BatState::StateName m_currentState;
	bool isNear = false;
};