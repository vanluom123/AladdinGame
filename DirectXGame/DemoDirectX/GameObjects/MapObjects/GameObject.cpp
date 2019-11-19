#include "GameObject.h"

GameObject::~GameObject()
{
}

GameObject::GameObject()
{
}

void GameObject::Update(float dt)
{
}

void GameObject::Draw(GVector3 position, RECT sourceRect, GVector2 scale, GVector2 transform, float angle, GVector2 rotationCenter, D3DXCOLOR colorKey)
{

}

void GameObject::Draw(GVector2 transform)
{
	this->Draw(GVector3(), RECT(), GVector2(), transform);
}
