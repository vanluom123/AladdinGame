#ifndef __GAME_DEBUG_H__
#define __GAME_DEBUG_H__

#include "Camera.h"

class GameDebugDraw 
{
public:
    GameDebugDraw();
	~GameDebugDraw();

    void Draw();
    void DrawLine(GVector2 lines[], int count);
    void DrawRect(RECT rect, Camera *camera = nullptr);
    LP_LINE LineDraw;
    //set do rong cua duong ve, mac dinh = 10
    void setLineSize(float width);
    void setColor(D3DCOLOR color);

private:
    LP_SPRITE mSpriteHandler;
    LP_3DDEVICE mddv;
    D3DCOLOR mColor;
};


#endif // __GAME_DEBUG_H__
