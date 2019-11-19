#pragma once

#include "../GameDefines/GameDefine.h"

class Viewport
{
	int _width;
	int _height;
	GVector3 _position;

public:
	Viewport(int width, int height);
	~Viewport();

	//center of camera
	void SetPosition(float x, float y);
	void SetPosition(GVector3 pos);

	int GetWidth();
	int GetHeight();

	GVector3 GetPosition();
	RECT GetBound();

	GVector2 getTranslate();

	void checkViewportWithMapWorld(class Player* pPlayer, class GameMap* pGameMap);

};

