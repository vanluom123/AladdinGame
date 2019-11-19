#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(const std::vector<InfoSprite>& listFrames) :
	Index(this),
	Loop(this),
	CurrentTotalTime(this)
{
	for (auto item : listFrames)
		m_listFrames.push_back(item);

	m_index = 0;
	m_totalTime = 0.0f;
	m_isLoop = true;
}

AnimatedSprite::~AnimatedSprite()
{
	if (!m_listFrames.empty())
		m_listFrames.clear();
}

void AnimatedSprite::update(float dt)
{
	if (m_listFrames.size() <= 1)
		return;

	m_totalTime += dt;

	if (m_totalTime < m_listFrames.at(m_index).getTimePerFrame())
		return;

	m_totalTime = 0.0f;
	this->next();

	if (m_index >= (int)m_listFrames.size())
	{
		if (m_isLoop)
			this->reset();
		else
		{
			m_index = (int)m_listFrames.size() - 1;
			m_totalTime = 0.0f;
		}
	}
}

void AnimatedSprite::render(SpriteSheet* spr, const GVector2& pos, const GVector2& trans, D3DCOLOR subColor)
{
	spr->setWidth(m_listFrames[m_index].getWidth());
	spr->setHeight(m_listFrames[m_index].getHeight());
	spr->setRect(m_listFrames[m_index].getRect());
	spr->setPosition(pos);
	spr->setTranslation(trans);

	spr->render(subColor);
}

void AnimatedSprite::next()
{
	m_index++;
}

void AnimatedSprite::reset()
{
	m_index = 0;
}