#pragma once

#include "../GameComponents/GameGlobal.h"
#include "../GameComponents/Sound.h"

class Entity
{
public:
	Entity();
	virtual ~Entity() = default;

	enum SideCollisions
	{
		Left,
		Right,
		Top,
		Bottom,
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
		NotKnow
	};

	struct CollisionReturn
	{
		bool IsCollided;
		RECT RegionCollision;
		SideCollisions sideCollision;
	};

	enum EntityTypes
	{
		None,
		Aladdin,
		AppleAladdin,
		Guard1,
		Guard2,
		Guard3,
		BatEnemy,
		CivilianEnemy1st,
		CivilianEnemy2nd,
		CivilianEnemy4th,
		Jafar,
		EnemiesWeapon,
		JafarWeapon1,
		JafarWeapon2,
		Ground,
		VerticalRope,
		HorizontalRope,
		Stair1,
		Stair2,
		Items,
		SpringBoard,   //Thanh gỗ chuyển Player sang Flippe.
		ChangeEnableStair,//Vị trí thay đổi xem có xét va chạm với Stair hay không
		Fire,
		FloatingGround,
		Camel,
		Flame,
		TYPE_PENDULUM,
		TYPE_TRAP,
		TYPE_BRICK,
		TYPE_BONE,
		TYPE_SKETELON,
		TYPE_MUMMIES,
		TYPE_MUMMIES_BONE
	};


	enum ItemTypes
	{
		Information,
		Store, 
		Apple,
		MagicLamp,
		GenieBonusLevel,
		OneUP, 
		ExtraHealth, 
		RestartPoint,
		AbuBonusLevel,
		SpendThese
	};

	enum FaceDirection
	{
		LEFT,
		RIGHT
	};

	ItemTypes ItemTag;//Tag nhan dien loai Item
	EntityTypes Tag; //Tag de nhan vien loai Entity
	FaceDirection faceDirection;


	virtual void Draw(D3DXVECTOR2 trans);

	virtual RECT GetBound();
	virtual RECT GetBoundBody();
	virtual RECT GetBoundWeapon();

	virtual FaceDirection getFaceDirection();

	virtual void SetPosition(float x, float y);
	virtual void SetPosition(D3DXVECTOR2 pos);
	virtual void SetPosition(D3DXVECTOR3 pos);
	virtual void AddPosition(D3DXVECTOR3 pos);
	virtual void AddPosition(D3DXVECTOR2 pos);
	virtual void AddPosition(float x, float y);

	virtual void SetWidth(int width);
	virtual int GetWidth();

	virtual void SetHeight(int height);
	virtual int GetHeight();

	virtual float GetVx();
	virtual void SetVx(float vx);
	virtual void AddVx(float vx);

	virtual float GetVy();
	virtual void SetVy(float vy);
	virtual void AddVy(float vy);

	virtual D3DXVECTOR3 GetPosition();

	virtual void Update(float dt);
	virtual void OnCollision(Entity* impactor, CollisionReturn data, SideCollisions side);
	virtual void OnCollision(Entity* impactor, CollisionReturn data);

	virtual void SetHealth(int hp);
	int GetHealth();

	virtual void TakeDamage(int Damage);

	virtual void SetTimeImmortal(float timeImmortal);
	virtual float GetTimeImmortal();

	bool IsDeleted();
	void SetDelete(bool flag);

	bool IsDraw();
	void SetDraw(bool flag);

	bool IsDestroy();
	void SetDestroy(bool flag);
	void SetMakeDamage(bool flag);
	bool IsMakeDamage();

	SideCollisions GetPhysicsBodySide() { return mSide; }
	void SetPhysicsBodySide(SideCollisions val) { mSide = val; }

	float GetPositionX() { return posX; }
	void SetPositionX(float val) { posX = val; }

	float GetPositionY() { return posY; }
	void SetPositionY(float val) { posY = val; }

protected:
	virtual void OnSetPosition(D3DXVECTOR3 pos);

	bool isDeleted = false;;
	bool isMakeDamage = false;
	bool isDraw = true;
	//Vị trí của Entity
	float posX, posY;

	//Vận tốc của Entity
	float vx, vy;

	//size của Entity
	float width, height;

	int HP;

	bool isDestroy = false;

	float timeImmortal;

	SideCollisions mSide;
};