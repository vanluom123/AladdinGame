#pragma once

#include <map>
#include "../GameDefines/GameDefine.h"
#include "../Utils/properties.h"

class Entity
{
protected:
	virtual void OnSetPosition(GVector3 pos) {}
	void reset();

public:

	Entity();
	virtual ~Entity();

	enum eSide
	{
		None = 1 << 0,
		Left = 1 << 1,
		Right = 1 << 2,
		Top = 1 << 3,
		Bottom = 1 << 4,
		TopLeft = 1 << 5,
		TopRight = 1 << 6,
		BottomLeft = 1 << 7,
		BottomRight = 1 << 8
	};

	void CheckCollision(Entity* entityOther, eSide side, float collisionTime);
	virtual void OnCollision(eSide side) {};

	struct CollisionReturn
	{
		bool _IsCollide;
		RECT _RegionCollision;
	};

	virtual RECT GetBounding();

	void AddPosition(GVector3 pos);
	void AddPosition(GVector2 pos);
	void AddPosition(float x, float y);

	void SetWidth(int width);
	int GetWidth();

	void SetHeight(int height);
	int GetHeight();

	float GetVx();
	void SetVx(float vx);
	void AddVx(float vx);

	float GetVy();
	void SetVy(float vy);
	void AddVy(float vy);

	// Property
	//__declspec(property(get = GetVx, put = SetVx)) float Veloctity;

	virtual void update(float dt);
	virtual void OnCollision(Entity* impactor, CollisionReturn data, eSide side);

	// PROPERTY
	GETSET(Entity, int, Height, { return _Height; }, { _Height = value; })
		GETSET(Entity, int, Width, { return _Width; }, { _Width = value; })

		GETSET(Entity, float, PositionX, { return _X; }, { _X = value; })
		GETSET(Entity, float, PositionY, { return _Y; }, { _Y = value; })

		GETSET(Entity, bool, Deleted, { return _IsDelete; }, { if (_IsDelete != value) { _IsDelete = value; } })

		GETSET(Entity, eID, Tag, { return _tag; }, { _tag = value; })

		GETSET(Entity, GVector2, Velocity,
			{
				return GVector2(_Vx, _Vy);
			},
			{
				_Vx = value.x;
				_Vy = value.y;
			})

		GETSET(Entity, GVector3, Position,
			{
				return GVector3(_X, _Y, 0);
			},
			{
				_X = value.x;
				_Y = value.y;
			})

		GETSET(Entity, eDirection, PhysicsBodySide, { return _physicsBodySide; }, { _physicsBodySide = value; })

protected:

	eDirection _physicsBodySide;

	float _X;
	float _Y;

	float _Vx;
	float _Vy;

	int _Width;
	int _Height;

	Entity* _ObjectX;
	Entity* _ObjectY;

	float _CollisionTimeMinX;
	float _CollisionTimeMinY;

	bool _IsDelete;
	bool _IsDestroy;

	eID _tag;

	eSide _SideX;
	eSide _SideY;
};