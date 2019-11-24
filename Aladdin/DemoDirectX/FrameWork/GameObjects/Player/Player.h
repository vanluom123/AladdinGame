#pragma once

#include "../../GameComponents/Animation.h"
#include "../../GameComponents/GameGlobal.h"
#include "../../GameComponents/Camera.h"
#include "../Entity.h"
#include "PlayerData.h"
#include "PlayerState.h"
#include "../../GameComponents/GameCollision.h"

class Player :public Entity
{
public:
	Player();
	~Player();

	enum MoveDirection
	{
		MoveToLeft, //chay tu phai sang trai
		MoveToRight, //chay tu trai sang phai
		None //dung im
	};

	void SetCamera(Camera* camera);

	void Update(float dt);
	void Draw(GVector3 position = GVector3(), RECT sourceRect = RECT(), GVector2 scale = GVector2(), GVector2 transform = GVector2(), float angle = 0, GVector2 rotationCenter = GVector2(), D3DXCOLOR colorKey = D3DCOLOR_XRGB(255, 255, 255));
	void SetState(PlayerState* newState);
	void OnCollision(Entity* impactor, Entity::CollisionReturn data, Entity::SideCollisions side);
	void OnCollision(Entity* impactor, Entity::CollisionReturn data);
	void OnNoCollisionWithBottom(float dt);

	void TakeDamage(int Damage);

	MoveDirection getMoveDirection();
	FaceDirection getFaceDirection();

	virtual RECT GetBound();
	virtual RECT GetBoundVerticalRope();
	virtual RECT GetBoundHorizontalRope();
	virtual RECT GetBoundBody();
	virtual RECT GetBoundWeapon();

	PlayerState::StateName Player::getState();

	void HandleKeyboard(std::map<int, bool> keys);
	void OnKeyPressed(int key);
	void OnKeyUp(int key);

	//true thi se lat nguoc anh theo truc y
	void SetReverse(bool flag);

	int GetNumApples();
	void ThrowApple();

	int GetNumLives();
	void SetLives(int live);

	int GetNumScore();
	void SetScore(int Score);

	int GetNumMoney();
	void SetMoney(int money);

	void SetRevivePoint(GVector2 revivePoint);
	void SetStartPoint(GVector2 startPoint);
	GVector2 GetRevivePoint();
	GVector2 GetStartPoint();

	bool IsAllowCut();
	bool IsAllowThrow();

	void SetAllowCut(bool flag);
	void SetAllowThrow(bool flag);

	Animation* GetCurrentAnimation();

	bool allowJump = true;
	bool allowMoveLeft;
	bool allowMoveRight;
	bool EnableStair1 = false;
	bool EnableStair2 = false;

protected:
	void changeAnimation(PlayerState::StateName state);

	float timeNoCollisionBottom = 0;
	float timeCreateFire = 0;
	float timeChangeStateStair;

	bool allowCut = true;
	bool allowThrow = true;
	bool mCurrentReverse;
	bool isFalling = false;


	PlayerData* mPlayerData;
	Camera* mCamera;
	Animation* mCurrentAnimation;
	Animation* mAnimationHanging;

	PlayerState::StateName mCurrentState;
	GVector2 RevivePoint;
	GVector2 StartPoint;

	//Aladdin Info
	int numApples = 10;
	int Lives;	//Số mạng của Aladdin
	int Score;	//Số điểm
	int Money;	//Số tiền? Cục màu đỏ
};
