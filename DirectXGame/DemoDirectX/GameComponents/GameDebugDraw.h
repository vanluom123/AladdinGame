#pragma once
#include "Viewport.h"

class GameDebugDraw 
{
public:
    GameDebugDraw();
    void Draw();
    void DrawLine(GVector2 lines[], int count);
    void DrawRect(RECT rect, Viewport *camera = nullptr);
    LPD3DXLINE LineDraw;
    //set do rong cua duong ve, mac dinh = 10
    void setLineSize(float width);
    void setColor(D3DCOLOR color);
    ~GameDebugDraw();

private:
    LPD3DXSPRITE mSpriteHandler;
    LPDIRECT3DDEVICE9 mddv;
    D3DCOLOR mColor;
};


