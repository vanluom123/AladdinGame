#pragma once
#include <vector>
#include "InfoSprite.h"
#include "SpriteSheet.h"
class AnimatedSprite
{
	std::vector<InfoSprite> m_listFrames;
	int m_index;
	float m_totalTime;
	bool m_isLoop;

public:
	AnimatedSprite() = default;
	AnimatedSprite(const std::vector<InfoSprite>& listFrames);
	~AnimatedSprite();

	void update(float dt);
	void render(SpriteSheet* spr, const GVector2& pos, const GVector2& trans = GVec2ZERO, D3DCOLOR subColor = D3DCOLOR_ARGB(255, 255, 255, 255));

	void next();
	void reset();

	GETSET(AnimatedSprite, int, Index, { return m_index; }, { m_index = value; })
		GET(AnimatedSprite, float, CurrentTotalTime, { return m_totalTime; })
		GETSET(AnimatedSprite, bool, Loop, { return m_isLoop; }, { m_isLoop = value; })
};

