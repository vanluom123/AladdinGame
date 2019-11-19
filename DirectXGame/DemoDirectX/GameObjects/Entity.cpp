#include "Entity.h"
#include "../GameComponents/GameCollision.h"

void Entity::reset()
{
	_SideX = None;
	_SideY = None;
	_CollisionTimeMinY = 1.0f;
	_CollisionTimeMinX = 1.0f;
}

// Constructor
Entity::Entity() :
	Tag(this),
	Deleted(this),
	Width(this),
	Height(this),
	PositionX(this),
	PositionY(this),
	Velocity(this),
	Position(this),
	PhysicsBodySide(this)
{
	_tag = UNKNOWN;
	_X = 0;
	_Y = 0;
	_Vx = 0;
	_Vy = 0;
	_Width = 0;
	_Height = 0;
	_CollisionTimeMinX = 1.0f;
	_CollisionTimeMinY = 1.0f;
	_SideX = None;
	_SideY = None;
	_IsDelete = false;
	_IsDestroy = false;
	_ObjectX = nullptr;
	_ObjectY = nullptr;
}

// Destructor
Entity::~Entity()
{}

void Entity::CheckCollision(Entity* entityOther, eSide side, float collisionTime)
{
	switch (side)
	{
	case Left:
	case Right:
		if (collisionTime < _CollisionTimeMinX)
		{
			_CollisionTimeMinX = collisionTime;
			_SideX = side;
			_ObjectX = entityOther;
		}
		break;

	case Top:
	case Bottom:
		if (collisionTime < _CollisionTimeMinY)
		{
			_CollisionTimeMinY = collisionTime;
			_SideY = side;
			_ObjectY = entityOther;
		}
		break;

	default: break;
	}
}

RECT Entity::GetBounding()
{
	RECT r;
	r.left = long(_X - _Width / 2);
	r.right = long(_X + _Width / 2);
	r.top = long(_Y - _Height / 2);
	r.bottom = long(_Y + _Height / 2);
	return r;
}

void Entity::OnCollision(Entity* impactor, CollisionReturn data, Entity::eSide side)
{
	_Vx = 0, _Vy = 0;
}

void Entity::update(float dt)
{
	if (_SideX != None)
	{
		if (_CollisionTimeMinY == 0.0f)
		{
			if (this->GetBounding().bottom <= _ObjectX->GetBounding().top || this->GetBounding().top >= _ObjectX->GetBounding().bottom)
			{
				_CollisionTimeMinX = 1.0f;
				_SideX = None;
			}
		}

		this->OnCollision(_SideX);
	}

	if (_SideY != None)
	{
		if (_CollisionTimeMinX == 0.0f)
		{
			if (this->GetBounding().right <= _ObjectY->GetBounding().left || this->GetBounding().left >= _ObjectY->GetBounding().right)
			{
				_CollisionTimeMinY = 1.0f;
				_SideY = None;
			}
		}

		this->OnCollision(_SideY);
	}

	//velocity = pixel / s
	_X += _Vx * dt * _CollisionTimeMinX;
	_Y += _Vy * dt * _CollisionTimeMinY;

	this->reset();
}

void Entity::AddPosition(GVector3 pos)
{
	_X = pos.x;
	_Y = pos.y;
}

void Entity::AddPosition(GVector2 pos)
{
	AddPosition(GVector3(pos));
}

void Entity::AddPosition(float x, float y)
{
	AddPosition(GVector3(x, y, 0));
}

void Entity::SetWidth(int width)
{
	_Width = width;
}

int Entity::GetWidth()
{
	return  _Width;
}

void Entity::SetHeight(int height)
{
	_Height = height;
}

int Entity::GetHeight()
{
	return  _Height;
}

float Entity::GetVx()
{
	return  _Vx;
}

void Entity::SetVx(float vx)
{
	_Vx = vx;
}

void Entity::AddVx(float vx)
{
	_Vx += vx;
}

float Entity::GetVy()
{
	return  _Vy;
}

void Entity::SetVy(float vy)
{
	_Vy = vy;
}

void Entity::AddVy(float vy)
{
	_Vy += vy;
}