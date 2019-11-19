#include "InfoSprite.h"

InfoSprite::InfoSprite(int width, int height, const RECT& r, float timePerFrame) :
	m_width(width),
	m_height(height),
	m_rect(r),
	m_timePerFrame(timePerFrame),
	Width(this),
	Height(this),
	TimePerFrame(this),
	Rect(this)
{}

InfoSprite::~InfoSprite()
{}