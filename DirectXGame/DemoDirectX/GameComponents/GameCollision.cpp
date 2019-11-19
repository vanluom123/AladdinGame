#include "GameCollision.h"

Entity::CollisionReturn GameCollision::RectAndRect(RECT rect1, RECT rect2)
{
	Entity::CollisionReturn result;

	if (!isCollide(rect1, rect2))
	{
		result._IsCollide = false;
		return result;
	}

	result._IsCollide = true;

	// Choose max Left
	result._RegionCollision.left = max(rect1.left, rect2.left);
	// Choose max right
	result._RegionCollision.right = min(rect1.right, rect2.right);
	// Choose min bottom
	result._RegionCollision.bottom = min(rect1.bottom, rect2.bottom);
	// Choose max top
	result._RegionCollision.top = max(rect1.top, rect2.top);

	return result;
}

bool GameCollision::PointAndRectangle(float x, float y, RECT rect)
{
	return !(x < rect.left || x > rect.right || y < rect.top || y > rect.bottom);
}

bool GameCollision::RectangleAndCircle(RECT rect, int circlex, int circley, int circleRadius)
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

Entity::eSide GameCollision::getSide(Entity* ent, Entity::CollisionReturn data)
{
	auto myRect = ent->GetBounding();
	auto otherRect = data._RegionCollision;

	float left = otherRect.left - myRect.right;
	float top = otherRect.top - myRect.bottom;
	float right = otherRect.right - myRect.left;
	float bottom = otherRect.bottom - myRect.top;

	// check collision
	if (left > 0 || right < 0 || top < 0 || bottom > 0)
		return Entity::None;

	float minX;
	float minY;
	Entity::eSide sideY;
	Entity::eSide sideX;

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
	{
		return sideX;
	}
	else
	{
		return sideY;
	}
}

GVector2 GameCollision::getDistance(Entity* ent1, Entity* ent2, float dt)
{
	GVector2 distance;
	distance.x = (ent1->GetVx() - ent2->GetVx()) * dt;
	distance.y = (ent1->GetVy() - ent2->GetVy()) * dt;
	return distance;
}

RECT GameCollision::getBroadphase(RECT entRect, GVector2 distance)
{
	RECT broad = entRect;

	if (distance.x < 0)
		broad.left = entRect.left + distance.x;
	else if (distance.x > 0)
		broad.right = entRect.right + distance.x;

	if (distance.y > 0)
		broad.bottom = entRect.bottom + distance.y;
	else if (distance.y < 0)
		broad.top = entRect.top + distance.y;

	return broad;
}

float GameCollision::sweptAABB(RECT entRect, RECT otherRect, GVector2 distance, Entity::eSide& side)
{
	float dxEntry, dxExit;
	float dyEntry, dyExit;

	if (distance.x > 0.0f)
	{
		dxEntry = otherRect.left - entRect.right;
		dxExit = otherRect.right - entRect.left;
	}
	else
	{
		dxEntry = otherRect.right - entRect.left;
		dxExit = otherRect.left - entRect.right;
	}

	if (distance.y > 0.0f)
	{
		dyEntry = otherRect.top - entRect.bottom;
		dyExit = otherRect.bottom - entRect.top;
	}
	else
	{
		dyEntry = otherRect.bottom - entRect.top;
		dyExit = otherRect.top - entRect.bottom;
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
		txEntry = dxEntry / (distance.x);
		txExit = dxExit / (distance.x);
	}

	if (distance.y == 0.0f)
	{
		tyEntry = -std::numeric_limits<float>::infinity();
		tyExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		tyEntry = dyEntry / (distance.y);
		tyExit = dyExit / (distance.y);
	}

	float entryTime = max(txEntry, tyEntry);
	float exitTime = min(txExit, tyExit);

	if (entryTime > exitTime || (txEntry < 0.0f && tyEntry < 0.0f) || txEntry > 1.0f || tyEntry > 1.0f)
	{
		side = Entity::None;
		return 1.0f;
	}

	if (txEntry < tyEntry)
	{
		if (dyEntry < 0.0f)
			side = Entity::Top;
		else
			side = Entity::Bottom;
	}
	else
	{
		if (dxEntry < 0.0f)
			side = Entity::Left;
		else
			side = Entity::Right;
	}

	return entryTime;
}

bool GameCollision::isCollide(RECT rect1, RECT rect2)
{
	return !(rect1.left > rect2.right || rect1.right < rect2.left || rect1.top > rect2.bottom || rect1.bottom < rect2.top);
}

Entity::eSide GameCollision::getSideCollision(Entity* e1, Entity* e2)
{
	RECT rect1 = e1->GetBounding();
	RECT rect2 = e2->GetBounding();

	float w = (e1->GetWidth() + e2->GetWidth()) / 2.0f;
	float h = (e1->GetHeight() + e2->GetHeight()) / 2.0f;

	float dx = e1->getPosition().x - e2->getPosition().y;
	float dy = e1->getPosition().x - e2->getPosition().y;

	if (abs(dx) <= w && abs(dy) <= h)
	{
		float wy = w * dy;
		float hx = h * dx;

		if (wy > hx)
		{
			if (wy > -hx)
			{
				return Entity::Top;
			}
			else
			{
				return Entity::Right;
			}
		}
		else if (wy > -hx)
		{
			return Entity::Left;
		}
		else
		{
			return Entity::Bottom;
		}
	}

	return Entity::None;
}

Entity::eSide GameCollision::getSideCollision(Entity* e1, Entity::CollisionReturn data)
{
	float xCenter = data._RegionCollision.left + (data._RegionCollision.right - data._RegionCollision.left) / 2.0f;
	float yCenter = data._RegionCollision.top + (data._RegionCollision.bottom - data._RegionCollision.top) / 2.0f;

	GVector2 cCenter = GVector2(xCenter, yCenter);
	GVector2 eCenter = e1->getPosition();

	if (e1->Tag == eID::PLAYER)
	{
		eCenter = D3DXVECTOR2(e1->getPosition().x, e1->getPosition().y - e1->GetHeight() / 2);
		//eCenter = D3DXVECTOR2((e1->GetBounding().left + e1->GetBounding().right) / 2, e1->GetBounding().bottom - e1->GetHeight() / 2);
	}

	GVector2 vec = cCenter - eCenter;

	D3DXVec2Normalize(&vec, &vec);

	if (vec.y < 0)
	{
		if (vec.x <= 0.35f && vec.x >= -0.35f)
		{
			return Entity::Top;
		}
		else if (vec.x > 0.35f && vec.x < 0.8f)
		{
			return Entity::TopRight;
		}
		else if (vec.x >= 0.8f)
		{
			return Entity::Right;
		}
		else if (vec.x < -0.35f && vec.x >= -0.8f)
		{
			return Entity::TopLeft;
		}
		else
		{
			return Entity::Left;
		}
	}
	else
	{
		if (vec.x <= 0.35f && vec.x >= -0.35)
		{
			return Entity::Bottom;
		}
		else if (vec.x > 0.35 && vec.x < 0.8)
		{
			return Entity::BottomRight;
		}
		else if (vec.x >= 0.8)
		{
			return Entity::Right;
		}
		else if (vec.x < -0.35f && vec.x > -0.8f)
		{
			return Entity::BottomLeft;
		}
		else
		{
			return Entity::Left;
		}
	}

	return Entity::None;
}