#include "GameCollision.h"

Entity::CollisionReturn GameCollision::rectCollide(RECT rect1, RECT rect2)
{
	Entity::CollisionReturn result;
	if (!AABBCheck(rect1, rect2))
	{
		result.IsCollided = false;
		return result;
	}

	result.IsCollided = true;
	result.RegionCollision.left = max(rect1.left, rect2.left);
	result.RegionCollision.top = max(rect1.top, rect2.top);
	result.RegionCollision.right = min(rect1.right, rect2.right);
	result.RegionCollision.bottom = min(rect1.bottom, rect2.bottom);
	return result;
}

bool GameCollision::pointCollideRect(float x, float y, RECT rect)
{
	return !(x < rect.left || x > rect.right || y < rect.top || y > rect.bottom);
}

bool GameCollision::rectCollideCircle(RECT rect, int circlex, int circley, int circleRadius)
{
	int px = circlex;
	int py = circley;

	if (px < rect.left)
		px = rect.left;
	else if (px > rect.right)
		px = rect.right;

	if (py > rect.bottom)
		py = rect.bottom;
	else if (py < rect.top)
		py = rect.top;

	int dx = px - circlex;
	int dy = py - circley;

	return (dx * dx + dy * dy) <= circleRadius * circleRadius;
}

Entity::SideCollisions GameCollision::GetSide(Entity* otherObject, Entity::CollisionReturn data)
{
	auto otherRect = otherObject->GetBound();

	float left = otherRect.left - data.RegionCollision.right;
	float top = otherRect.top - data.RegionCollision.bottom;
	float right = otherRect.right - data.RegionCollision.left;
	float bottom = otherRect.bottom - data.RegionCollision.top;

	// kt va chạm
	if (left > 0 || right < 0 || top > 0 || bottom < 0)
		return Entity::NotKnow;

	float minX;
	float minY;
	Entity::SideCollisions sideY;
	Entity::SideCollisions sideX;

	if (top > abs(bottom))
	{
		minY = bottom;
		sideY = Entity::Bottom;
	}
	else
	{
		minY = top;
		sideY = Entity::Top;
	}


	if (abs(left) > right)
	{
		minX = right;
		sideX = Entity::Right;
	}
	else
	{
		minX = left;
		sideX = Entity::Left;
	}

	if (abs(minX) < abs(minY))
		return sideX;
	else
		return sideY;
}

float GameCollision::sweptAABB(Entity* myObject, Entity* otherObject, Entity::SideCollisions& direction, float dt)
{
	RECT myRect = myObject->GetBound();
	RECT otherRect = otherObject->GetBound();

	RECT broadphaseRect = getSweptBroadphaseRect(myObject, dt);
	if (!AABBCheck(broadphaseRect, otherRect))
	{
		direction = Entity::SideCollisions::NotKnow;
		return 1.0f;
	}

	// SweptAABB
	// vận tốc mỗi frame
	GVector2 otherVeloc = GVector2(otherObject->GetVx() * dt / 1000, otherObject->GetVy() * dt / 1000);
	GVector2 myVelocity = GVector2(myObject->GetVx() * dt / 1000, myObject->GetVy() * dt / 1000);
	GVector2 velocity = myVelocity;

	if (otherVeloc != GVector2(0, 0))
	{
		velocity = otherVeloc - myVelocity;
	}

	float _dxEntry;
	float _dxExit;
	if (velocity.x > 0)
	{
		_dxEntry = otherRect.left - myRect.right;
		_dxExit = otherRect.right - myRect.left;
	}
	else
	{
		_dxEntry = otherRect.right - myRect.left;
		_dxExit = otherRect.left - myRect.right;
	}

	float _dyEntry;
	float _dyExit;
	if (velocity.y > 0)
	{
		_dyEntry = otherRect.bottom - myRect.top;
		_dyExit = otherRect.top - myRect.bottom;
	}
	else
	{
		_dyEntry = otherRect.top - myRect.bottom;
		_dyExit = otherRect.bottom - myRect.top;
	}

	float _txEntry;
	float _txExit;
	if (velocity.x == 0)
	{
		_txEntry = -std::numeric_limits<float>::infinity();
		_txExit = std::numeric_limits<float>::infinity();
	}

	else
	{
		_txEntry = _dxEntry / velocity.x;
		_txExit = _dxExit / velocity.x;
	}

	float _tyEntry;
	float _tyExit;
	if (velocity.y == 0)
	{
		_tyEntry = -std::numeric_limits<float>::infinity();
		_tyExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		_tyEntry = _dyEntry / velocity.y;
		_tyExit = _dyExit / velocity.y;
	}

	float entryTime = max(_txEntry, _tyEntry);
	float exitTime = min(_txExit, _tyExit);

	if (entryTime > exitTime || _txEntry < 0.0f && _tyEntry < 0.0f || _txEntry > 1.0f || _tyEntry > 1.0f)
	{
		direction = Entity::SideCollisions::NotKnow;
		return 1.0f;
	}

	if (_txEntry > _tyEntry)
	{
		if (_dxEntry < 0.0f)
			direction = Entity::Right;
		else
			direction = Entity::Left;
	}
	else
	{
		if (_dyEntry < 0.0f)
			direction = Entity::Top;
		else
			direction = Entity::Bottom;
	}

	return entryTime;
}

RECT GameCollision::getSweptBroadphaseRect(Entity* myObject, float dt)
{
	// vận tốc mỗi frame
	auto velocity = GVector2(myObject->GetVx() * dt / 1000, myObject->GetVy() * dt / 1000);
	auto myRect = myObject->GetBound();

	RECT rect;
	rect.top = velocity.y > 0 ? myRect.top + velocity.y : myRect.top;
	rect.bottom = velocity.y > 0 ? myRect.bottom : myRect.bottom + velocity.y;
	rect.left = velocity.x > 0 ? myRect.left : myRect.left + velocity.x;
	rect.right = velocity.y > 0 ? myRect.right + velocity.x : myRect.right;

	return rect;
}

bool GameCollision::AABBCheck(RECT rect1, RECT rect2)
{
	auto left = rect1.left - rect2.right;
	auto top = rect1.top - rect2.bottom;
	auto right = rect1.right - rect2.left;
	auto bottom = rect1.bottom - rect2.top;

	return !(left > 0 || right < 0 || top > 0 || bottom < 0);
}


Entity::SideCollisions GameCollision::getSideCollision(Entity* e1, Entity* e2)
{
	RECT rect1 = e1->GetBound();
	RECT rect2 = e2->GetBound();

	float w = (e1->GetWidth() + e2->GetWidth()) / 2.0f;
	float h = (e1->GetHeight() + e2->GetHeight()) / 2.0f;

	float dx = e1->GetPosition().x - e2->GetPosition().y;
	float dy = e1->GetPosition().x - e2->GetPosition().y;

	if (abs(dx) <= w && abs(dy) <= h)
	{
		/* co va cham*/
		float wy = w * dy;
		float hx = h * dx;

		if (wy > hx)
		{
			if (wy > -hx)
			{
				/*va cham phia tren e1*/
				return Entity::Top;
			}
			else
			{
				/*va cham phia ben phai e1*/
				return Entity::Right;
			}
		}
		else if (wy > -hx)
		{
			/*va cham ben trai e1*/
			return Entity::Left;
		}
		else
		{
			/*va cham phia duoi e1*/
			return Entity::Bottom;
		}
	}
}

Entity::SideCollisions GameCollision::getSideCollision(Entity* e1, Entity::CollisionReturn data)
{
	float xCenter = data.RegionCollision.left + (data.RegionCollision.right - data.RegionCollision.left) / 2.0f;
	float yCenter = data.RegionCollision.top + (data.RegionCollision.bottom - data.RegionCollision.top) / 2.0f;
	GVector2 eCenter = e1->GetPosition();
	GVector2 cCenter = GVector2(xCenter, yCenter);
	if (e1->Tag == Entity::EntityTypes::Aladdin || e1->Tag == Entity::EntityTypes::Camel)
	{
		eCenter = GVector2(e1->GetPosition().x, e1->GetPosition().y - e1->GetHeight() / 2);
		eCenter = GVector2((e1->GetBound().left + e1->GetBound().right) / 2, e1->GetBound().bottom - e1->GetHeight() / 2);
	}


	//lay vector noi tam Entity va CollisionRect
	GVector2 vec = cCenter - eCenter;

	//chuan hoa vector
	D3DXVec2Normalize(&vec, &vec);

	/*
	- neu vector chuan hoa co y > 0 =>nam phia ben tren Entity
	- neu vector chuan hoa co y < 0 =>nam phia ben duoi Entity
	- neu vector chuan hoa co x > 0 => nam phia ben phai Entity
	- neu vector chuan hoa co x < 0 => nam phia ben trai Entity
	*/

	if (vec.y < 0)
	{
		//va cham phia ben tren
		//lay cos cua goc neu ma nam trong khoang goc 70 -> 110 thi va cham top
		if (vec.x <= 0.35f && vec.x >= -0.35f)
		{
			return Entity::Top;
		}
		else if (vec.x > 0.35f && vec.x < 0.8f)
		{
			//goc trong khoang 35 -> 70 phia ben top - right
			return Entity::TopRight;
		}
		else if (vec.x >= 0.8f)
		{
			return Entity::Right;
		}
		else if (vec.x < -0.35f && vec.x >= -0.8f)
		{
			//va cham phia top - left
			return Entity::TopLeft;
		}
		else
		{
			return Entity::Left;
		}
	}
	else
	{
		//va cham phia ben duoi
		//lay cos cua goc neu ma nam trong khoang goc 70 -> 110 thi va cham top
		if (vec.x <= 0.35f && vec.x >= -0.35)
		{
			return Entity::Bottom;
		}
		else if (vec.x > 0.35 && vec.x < 0.8)
		{
			//goc trong khoang 35 -> 70 phia ben top - right
			return Entity::BottomRight;
		}
		else if (vec.x >= 0.8)
		{
			return Entity::Right;
		}
		else if (vec.x < -0.35f && vec.x > -0.8f)
		{
			//va cham phia top - left
			return Entity::BottomLeft;
		}
		else
		{
			return Entity::Left;
		}
	}

	return Entity::NotKnow;
}