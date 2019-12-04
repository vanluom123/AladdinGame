#pragma once

#include <cstdlib>
#include "../GameObjects/Entity.h"
#include <limits>

class GameCollision
{
public:

	// kiem tra va cham giua 2 hinh chu nhat
	static Entity::CollisionReturn rectCollide(RECT rect, RECT rect2);

	// AABBCheck
	static bool AABBCheck(RECT rect1, RECT rect2);

	// neu rectagle qua to thi va cham co the bi sai
	static Entity::SideCollisions getSideCollision(Entity* e1, Entity* e2);

	// kiem tra vi tri va cham dua vao vung va cham cua 2 vat the
	static Entity::SideCollisions getSideCollision(Entity* e1, Entity::CollisionReturn data);

	// kiem tra 1 diem co nam trong hinh chu nhat hay khong
	static bool pointCollideRect(float x, float y, RECT rect);

	// hinh chu nhat va hinh tron
	static bool rectCollideCircle(RECT rect, int circlex, int circley, int circleRadius);

	// Get distance
	static GVector2 getDistance(Entity* myObject, Entity* otherObject, float dt);

	// Broadphase
	static RECT getSweptBroadphaseRect(RECT object, GVector2 distance);

	// SweptAABB
	static float sweptAABB(RECT myObject, RECT otherObject, Entity::SideCollisions& side, GVector2 distance);
};