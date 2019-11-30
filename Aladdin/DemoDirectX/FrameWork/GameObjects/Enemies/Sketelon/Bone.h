#ifndef __BONE_H__
#define __BONE_H__
#include "../../Entity.h"
#include "../../../GameComponents/Animation.h"
#include "../../../GameComponents/GameDebugDraw.h"

class GameMap;
class Bone : public Entity
{

protected:
	Animation* _anim;
	GameMap* _gameMap;
	GameDebugDraw* _debug;

public:
	Bone(GVector2 position);
	Bone();
	~Bone();

	void initialize(GVector2 position);

	void Draw(GVector2 trans) override;
	void Draw(GVector2 position, GVector2 trans);
	void Update(float dt) override;
	RECT GetBound() override;
	void OnCollision(Entity* impactor);

	void set_GameMap(GameMap* val);
};

#endif // __BONE_H__
