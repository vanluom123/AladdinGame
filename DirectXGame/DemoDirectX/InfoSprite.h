#ifndef __INFO_SPRITE_H__
#define __INFO_SPRITE_H__

#include <d3d9.h>
#include "Utils/properties.h"

class InfoSprite
{
	int m_width;
	int m_height;
	RECT m_rect;
	float m_timePerFrame;

public:
	InfoSprite(int width, int height, const RECT& r, float timePerFrame = 0.1f);
	~InfoSprite();

	// Property
	GETSET(InfoSprite, int, Width, { return m_width; }, { m_width = value; })
		GETSET(InfoSprite, int, Height, { return m_height; }, { m_height = value; })
		GETSET(InfoSprite, float, TimePerFrame, { return m_timePerFrame; }, { m_timePerFrame = value; })
		GETSET(InfoSprite, RECT, Rect, { return m_rect; }, { m_rect = value; })
};

#endif // __INFO_SPRITE_H__
