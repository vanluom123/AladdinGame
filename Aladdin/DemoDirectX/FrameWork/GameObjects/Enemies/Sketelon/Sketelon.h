#ifndef __SKETELON_H__
#define __SKETELON_H__
#include "../../Entity.h"
#include "../../../GameComponents/Animation.h"
#include "Bone.h"

class GameMap;
class Sketelon : public Entity
{

public:
	Sketelon(GVector2 position);
	Sketelon() = default;
	~Sketelon();

	enum eSketelonStatus
	{
		eSke_Normal,
		eSke_Explode,
		eSke_Death
	};

	void initialize(GVector2 position);

	void Draw(GVector2 trans) override;
	void Update(float dt) override;
	void OnCollision(Entity* impactor, CollisionReturn data, SideCollisions side) override;
	RECT GetBound() override;

	void CapNhat_PhatNo(float dt);
	void PhatNo();

	void set_GameMap(GameMap* val);

private:
	eSketelonStatus _status;
	Animation* _anim;
	Animation* _animDeath;
	Animation* _currentAnim;
	Bone* _bone;

	GVector2 _range_explode;

	GameMap* _gameMap;

	GVector2 _position_explode[8];
	GVector2 _velocity_explode[8] = {
		GVector2(100.0f, 0.0f),
		GVector2(100.0f * 0.707f, -100.0f * 0.707f),
		GVector2(0.0f, -100.f),
		GVector2(-100.0f * 0.707f, -100.0f * 0.707f),
		GVector2(-100.0f, 0.0f),
		GVector2(-100.0f * 0.707f, 100.0f * 0.707f),
		GVector2(0.0f, 100.0f),
		GVector2(100.0f * 0.707f, 100.0f * 0.707f)
	};
};

#endif // __SKETELON_H__
