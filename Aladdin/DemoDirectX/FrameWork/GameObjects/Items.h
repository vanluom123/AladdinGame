#ifndef __ITEMS_H__
#define __ITEMS_H__

#include "../GameComponents/Animation.h"
#include "Entity.h"

class Items : public Entity
{
public:
	
	~Items();
	Items();
	Items(GVector3 position);
	Items(GVector3 position, ItemTypes type);
	Items(GVector2 position, ItemTypes type);

	void Update(float dt);
	void Draw(GVector3 position = GVector3(), RECT sourceRect = RECT(), GVector2 scale = GVector2(), GVector2 transform = GVector2(), float angle = 0, GVector2 rotationCenter = GVector2(), D3DXCOLOR colorKey = D3DCOLOR_XRGB(255, 255, 255));
	void Draw(GVector3 transform);
	void Draw(GVector2 transform);
	void OnCollision(Entity *impactor, Entity::CollisionReturn data, Entity::SideCollisions side) override;
	RECT GetBound();

protected:
	bool init(GVector3 position);
	bool init(GVector3 position, ItemTypes type);
	void OnSetPosition(GVector3 poks);
	Animation *mAnimation;
};
#endif // __ITEMS_H__
