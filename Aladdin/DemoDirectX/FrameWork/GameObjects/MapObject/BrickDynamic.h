#ifndef __BRICK_H__
#define __BRICK_H__
#include "../Entity.h"
#include "../../GameComponents/Animation.h"
#include "../../GameComponents/GameDebugDraw.h"

class BrickDynamic : public Entity
{

protected:
	Animation* _anim;
	float _time;
	GameDebugDraw* _debug;
	Camera* _camera;

	int _brickStatus;

public:
	BrickDynamic() = default;
	~BrickDynamic() = default;
	BrickDynamic(D3DXVECTOR2 position);
	void initialize(D3DXVECTOR2 position);

	void Draw(D3DXVECTOR2 trans) override;
	void Draw(D3DXVECTOR3 position = D3DXVECTOR3(),
		RECT sourceRect = RECT(),
		D3DXVECTOR2 scale = D3DXVECTOR2(),
		D3DXVECTOR2 transform = D3DXVECTOR2(),
		float angle = 0,
		D3DXVECTOR2 rotationCenter = D3DXVECTOR2(),
		D3DXCOLOR colorKey = D3DCOLOR_XRGB(255, 255, 255));

	RECT GetBound() override;
	void Update(float dt) override;

	inline Animation* GetAnimation() const {
		return _anim;
	}

	inline void SetCamera(Camera* camera) {
		_camera = camera;
	}

	void SetStatus(const char* val)
	{
		_brickStatus = atoi(val);
		if (_brickStatus == 0)
			_anim->mCurrentIndex = 1;
		else if (_brickStatus == 1)
			_anim->mCurrentIndex = 5;
	}
};

#endif
