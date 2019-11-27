#include "BrickDynamic.h"

BrickDynamic::BrickDynamic(D3DXVECTOR2 position)
{
	initialize(position);
}

void BrickDynamic::initialize(D3DXVECTOR2 position)
{
	_debug = new GameDebugDraw();
	isDeleted = false;
	isDraw = true;
	_brickStatus = 0;
	_time = 0;
	isDestroy = false;
	Tag = Entity::TYPE_BRICK;
	_anim = new Animation(eID::BRICK_TRAP_PENDULUM, "brick_", 5, 0.5f);
	SetPosition(position);
	SetWidth(_anim->GetSprite()->GetWidth());
	SetHeight(_anim->GetSprite()->GetHeight());
}

void BrickDynamic::Draw(D3DXVECTOR2 trans)
{
	_debug->DrawRect(this->GetBound());
	_anim->Draw(GetPosition(), RECT(), D3DXVECTOR2(), trans);
}

void BrickDynamic::Draw(D3DXVECTOR3 position, RECT sourceRect, D3DXVECTOR2 scale, D3DXVECTOR2 transform, float angle, D3DXVECTOR2 rotationCenter, D3DXCOLOR colorKey)
{
	_debug->DrawRect(this->GetBound());
	_anim->Draw(position, sourceRect, scale, transform, angle, rotationCenter, colorKey);
}

RECT BrickDynamic::GetBound()
{
	RECT r = RECT();

	POINT tmp = _anim->GetSprite()->GetAnchorPoint();
	if (tmp.x == -1 && tmp.y == -1)
	{
		r.left = posX - 38 / 2;
		r.top = posY - 21 / 2;
		r.right = r.left + 38;
		r.bottom = r.top + 21;
	}
	else
	{
		r.left = posX - tmp.x;
		r.top = posY - tmp.y;
		r.right = r.left + 38;
		r.bottom = r.top + 21;
	}

	return r;
}

void BrickDynamic::Update(float dt)
{
	_time += dt;

	if (_brickStatus == 0)
	{
		if (_time > 5.0f)
		{
			_anim->Update(dt);

			if (_anim->mCurrentIndex == 5)
			{
				_brickStatus = 1;
				_time = 0;
			}
		}
	}
	else if (_brickStatus == 1)
	{
		if (_time > 5.0f)
		{
			_anim->Update(dt, true);

			if (_anim->mCurrentIndex == 1)
			{
				_brickStatus = 0;
				_time = 0;
			}
		}
	}
}