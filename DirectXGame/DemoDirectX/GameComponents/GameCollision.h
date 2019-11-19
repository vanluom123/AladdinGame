#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include "../GameObjects/Entity.h"

class GameCollision
{
public:
    
	// Rectangle and rectangle
    static Entity::CollisionReturn RectAndRect(RECT rect, RECT rect2);

    static bool isCollide(RECT rect1, RECT rect2);

    static Entity::eSide getSideCollision(Entity *e1, Entity *e2);

    // Side collision
    static Entity::eSide getSideCollision(Entity *e1, Entity::CollisionReturn data);

    // Check point in Rectangle
    static bool PointAndRectangle(float x, float y, RECT rect);

    // Rectangle and Circle
    static bool RectangleAndCircle(RECT rect, int circlex, int circley, int circleRadius);

	// Side that player could collide
	static Entity::eSide getSide(Entity* ent, Entity::CollisionReturn data);

	static GVector2 getDistance(Entity* ent1, Entity* ent2, float dt);

	static RECT getBroadphase(RECT entRect, GVector2 distance);

	static float sweptAABB(RECT entRect, RECT otherRect, GVector2 distance, Entity::eSide& side);
};