
#ifndef __TRAP_H__
#define __TRAP_H__

#include "../Entity.h"
#include "../../GameComponents/Animation.h"

class Trap : public Entity
{

private:

	float _time;
	Animation* _anim;
	int _status;

public:

	Trap() = default;
	Trap(D3DXVECTOR2 position);
	~Trap();

	void Update(float dt) override;
	void Draw(D3DXVECTOR2 trans);
	void Draw(D3DXVECTOR3 position = D3DXVECTOR3(),
		RECT sourceRect = RECT(),
		D3DXVECTOR2 scale = D3DXVECTOR2(),
		D3DXVECTOR2 transform = D3DXVECTOR2(),
		float angle = 0,
		D3DXVECTOR2 rotationCenter = D3DXVECTOR2(),
		D3DXCOLOR colorKey = D3DCOLOR_XRGB(255, 255, 255));
	RECT GetBound() override;
	void OnCollision(Entity* impactor, CollisionReturn data, SideCollisions side) override;

public:

	Animation* GetAnim()
	{
		return _anim;
	}

	void SetStatus(const char* val)
	{
		_status = atoi(val);
		if (_status == 0)
			_anim->mCurrentIndex = 1;
		else if (_status == 1)
			_anim->mCurrentIndex = 6;
	}

};

#endif

