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

GVector2 GameCollision::getDistance(Entity* myObject, Entity* otherObject, float dt)
{
	GVector2 distance;
	distance.x = (myObject->GetVx() - otherObject->GetVx()) * dt;
	distance.y = (myObject->GetVy() - otherObject->GetVy()) * dt;
	return distance;
}

RECT GameCollision::getSweptBroadphaseRect(RECT object, GVector2 distance)
{
	RECT broad = object;

	if (distance.x < 0)
		broad.left = object.left + distance.x;
	else if (distance.x > 0)
		broad.right = object.right + distance.x;

	if (distance.y > 0)
		broad.bottom = object.bottom + distance.y;
	else if (distance.y < 0)
		broad.top = object.top + distance.y;

	return broad;
}

float GameCollision::sweptAABB(RECT myRect, RECT otherRect, Entity::SideCollisions& side, GVector2 distance)
{
	float dxEntry, dxExit;
	float dyEntry, dyExit;

	if (distance.x > 0.0f)
	{
		dxEntry = otherRect.left - myRect.right;
		dxExit = otherRect.right - myRect.left;
	}
	else
	{
		dxEntry = otherRect.right - myRect.left;
		dxExit = otherRect.left - myRect.right;
	}

	if (distance.y > 0.0f)
	{
		dyEntry = otherRect.top - myRect.bottom;
		dyExit = otherRect.bottom - myRect.top;
	}
	else
	{
		dyEntry = otherRect.bottom - myRect.top;
		dyExit = otherRect.top - myRect.bottom;
	}

	float txEntry, txExit;
	float tyEntry, tyExit;

	if (distance.x == 0.0f)
	{
		txEntry = -std::numeric_limits<float>::infinity();
		txExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		txEntry = dxEntry / distance.x;
		txExit = dxExit / distance.x;
	}

	if (distance.y == 0.0f)
	{
		tyEntry = -std::numeric_limits<float>::infinity();
		tyExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		tyEntry = dyEntry / distance.y;
		tyExit = dyExit / distance.y;
	}

	float entryTime = max(txEntry, tyEntry);
	float exitTime = min(txExit, tyExit);

	if (entryTime > exitTime || (txEntry < 0.0f && tyEntry < 0.0f) || txEntry > 1.0f || tyEntry > 1.0f)
	{
		side = Entity::SideCollisions::NotKnow;
		return 1.0f;
	}

	if (txEntry > tyEntry)
	{
		if (dxEntry > 0.0f)
			side = Entity::SideCollisions::Right;
		else
			side = Entity::SideCollisions::Left;
	}
	else
	{
		if (dyEntry > 0.0f)
			side = Entity::SideCollisions::Top;
		else
			side = Entity::SideCollisions::Bottom;
	}

	return entryTime;
}

bool GameCollision::AABBCheck(RECT rect1, RECT rect2)
{
	float left = rect1.left - rect2.right;
	float top = rect1.bottom - rect2.top;
	float right = rect1.right - rect2.left;
	float bottom = rect1.top - rect2.bottom;

	return !(left > 0 || right < 0 || top < 0 || bottom > 0);
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

	return Entity::NotKnow;
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