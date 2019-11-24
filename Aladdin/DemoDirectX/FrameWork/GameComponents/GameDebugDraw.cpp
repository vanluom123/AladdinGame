#include "GameDebugDraw.h"
#include "GameGlobal.h"

GameDebugDraw::GameDebugDraw()
{
    mSpriteHandler = GameGlobal::GetCurrentSpriteHandler();
    mSpriteHandler->GetDevice(&mddv);
    D3DXCreateLine(mddv, &LineDraw);
    
    //set size cho line
    LineDraw->SetWidth(1);

    //set mau cho line
    mColor = D3DCOLOR_XRGB(255, 0, 0);
}

GameDebugDraw::~GameDebugDraw()
{}

void GameDebugDraw::Draw()
{}

void GameDebugDraw::setLineSize(float width)
{
    LineDraw->SetWidth(width);
}

void GameDebugDraw::DrawLine(GVector2 lines[], int count)
{
    LineDraw->Begin();
    LineDraw->Draw(lines, count, mColor);
    LineDraw->End();
}

void GameDebugDraw::DrawRect(RECT rect, Camera *camera)
 {
    GVector3 trans = GVector3(0, 0, 0);

    if (camera)
    {
        trans = GVector3(GameGlobal::GetWidth() / 2, GameGlobal::GetHeight() / 2, 0) - camera->GetPosition();
    }    

    GVector2 lines[] = { GVector2(rect.left + trans.x, rect.top + trans.y),
                            GVector2(rect.right + trans.x, rect.top + trans.y), 
                            GVector2(rect.right + trans.x, rect.bottom + trans.y), 
                            GVector2(rect.left + trans.x, rect.bottom + trans.y), 
                            GVector2(rect.left + trans.x, rect.top + trans.y) };

    DrawLine(lines, 5);
}

void GameDebugDraw::setColor(D3DCOLOR color)
{
    mColor = color;
}