#include "../GameComponents/Animation.h"
#include "../GameControllers/SpriteManager.h"

Animation::Animation() 
{}

Animation::Animation(eID eId, string nameAnimation, int totalFrame, float timePerFrame)
{
	this->_InitWithAnimation(eId, nameAnimation, totalFrame, timePerFrame);
}

void Animation::_InitWithAnimation(eID eId, string nameAnimation, int totalFrame, float timePerFrame)
{
	_eID = eId;
	_pSprite = SpriteManager::getInstance()->getSprite(_eID);
	_timePerFrame = timePerFrame;
	_totalFrame = totalFrame;
	_nameAnimation = nameAnimation;
	_currentIndex = 1;
	_srcRect = SpriteManager::getInstance()->getSourceRect(_eID, _nameAnimation + to_string(_currentIndex));

	_frameW = _srcRect.right - _srcRect.left;
	_frameH = _srcRect.bottom - _srcRect.top;

	_pSprite->setWidth(_frameW);
	_pSprite->setHeight(_frameH);

	_pSprite->setSrcRect(_srcRect);
	_pSprite->setAnchorPoint(SpriteManager::getInstance()->getAnchorPoint(_eID, _nameAnimation + to_string(_currentIndex)));
}

Animation::~Animation()
{
	delete _pSprite;
}

void Animation::setFlipVertical(bool flag)
{
	_pSprite->setFlipVertical(flag);
}

void Animation::setFlipHorizontal(bool flag)
{
	_pSprite->setFlipHorizontal(flag);
}

GVector2 Animation::getScale()
{
	return _pSprite->getScale();
}

void Animation::setScale(GVector2 scale)
{
	_pSprite->setScale(scale);
}

float Animation::getRotation()
{
	return _pSprite->getRotation();
}

void Animation::setRotation(float rotation) // by radian
{
	_pSprite->setRotation(rotation);
}

GVector2 Animation::getRotationCenter()
{
	return _pSprite->getRotationCenter();
}

void Animation::setRotationCenter(GVector2 rotationCenter)
{
	_pSprite->setRotationCenter(rotationCenter);
}

GVector2 Animation::getTranslation()
{
	return _pSprite->getTranslation();
}

void Animation::setTranslation(GVector2 translation)
{
	_pSprite->setTranslation(translation);
}

void Animation::update(float dt)
{
	if (_totalFrame <= 1)
		return;
	if (_currentTotalTime >= _timePerFrame)
	{
		_currentTotalTime -= _timePerFrame;

		_currentIndex++;
		if (_enableLoop)
		{
			if (_startLoop == _stopLoop || _currentIndex > _stopLoop)
				_currentIndex = _startLoop;
		}
		else if (_currentIndex > _totalFrame)
		{
			if (_enableStop == false)
				_currentIndex = 1;
			else _currentIndex = _totalFrame;
		}

		_srcRect = SpriteManager::getInstance()->getSourceRect(_eID, _nameAnimation + to_string(_currentIndex));
		_pSprite->setAnchorPoint(SpriteManager::getInstance()->getAnchorPoint(_eID, _nameAnimation + to_string(_currentIndex)));

		_pSprite->setWidth(_srcRect.right - _srcRect.left);
		_pSprite->setHeight(_srcRect.bottom - _srcRect.top);
		_pSprite->setSrcRect(_srcRect);
	}
	else
		_currentTotalTime += dt;
}

void Animation::update(float dt, bool isDecrease)
{
	if (_totalFrame <= 1)
		return;
	if (_currentTotalTime >= _timePerFrame)
	{
		_currentTotalTime -= _timePerFrame;
		_currentIndex--;
		if (_currentIndex <= 0)
		{
			_currentIndex = _totalFrame;
		}

		_srcRect = SpriteManager::getInstance()->getSourceRect(_eID, _nameAnimation + to_string(_currentIndex));

		_pSprite->setWidth(_srcRect.right - _srcRect.left);
		_pSprite->setHeight(_srcRect.bottom - _srcRect.top);
		_pSprite->setSrcRect(_srcRect);
	}
	else
		_currentTotalTime += dt;
}

void Animation::draw(GVector3 position, RECT sourceRect, GVector2 scale,
	GVector2 transform, float angle, GVector2 rotationCenter, D3DXCOLOR colorKey)
{
	_pSprite->render(position, sourceRect, scale, transform, angle, rotationCenter, colorKey);
}


void Animation::draw(GVector2 translate)
{
	_pSprite->render(GVector3(), RECT(), GVector2(), translate);
}
void Animation::setPosition(GVector3 pos)
{
	_pSprite->setPosition(pos);
}

void Animation::setPosition(float x, float y)
{
	setPosition(GVector3(x, y, 0));
}

void Animation::setPosition(GVector2 pos)
{
	setPosition(GVector3(pos));
}

Sprite* Animation::getSprite()
{
	return _pSprite;
}


void Animation::reset()
{
	_currentIndex = 0;
}

void Animation::CopyAnimation(Animation* animation)
{
	this->_currentIndex = 1;
	this->_currentTotalTime = animation->_currentTotalTime;
	this->_eID = animation->_eID;
	this->_frameH = animation->_frameH;
	this->_frameW = animation->_frameW;
	this->_nameAnimation = animation->_nameAnimation;
	this->_srcRect = animation->_srcRect;
	this->_pSprite = animation->_pSprite;
	this->_timePerFrame = animation->_timePerFrame;
	this->_totalFrame = animation->_totalFrame;
	this->_enableStop = animation->_enableStop;
}
void Animation::setStop(bool flag)
{
	_enableStop = flag;
}

void Animation::setLoop(bool flag, int start, int stop)
{
	_enableLoop = flag;
	_startLoop = start;
	_stopLoop = stop;
}
POINT Animation::getAnchorPoint()
{
	return this->_pSprite->getAnchorPoint();
}
