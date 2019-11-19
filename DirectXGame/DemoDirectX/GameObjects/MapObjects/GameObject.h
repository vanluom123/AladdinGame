#pragma once

#include "../../GameDefines/GameDefine.h"
//vien gach
class GameObject
{
public:
    ~GameObject();

protected:
    GameObject();

    void Update(float dt);
    void Draw(GVector3 position = GVector3(), RECT sourceRect = RECT(), GVector2 scale = GVector2(), GVector2 transform = GVector2(), float angle = 0, GVector2 rotationCenter = GVector2(), D3DXCOLOR colorKey = D3DCOLOR_XRGB(255, 255, 255));
    void Draw(GVector2 transform);
};      
