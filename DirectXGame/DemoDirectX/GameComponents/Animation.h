#ifndef __ANIMATION__
#define __ANIMATION__

#include "Sprite.h"

class Animation
{
public:
	Animation(eID eId, string nameAnimation, int totalFrame, float timePerFrame = 0.1f);
	Animation();

	void update(float dt);
	void update(float dt, bool isDecrease);
	void draw(GVector3 position = GVector3(), RECT sourceRect = RECT(), GVector2 scale = GVector2(), GVector2 transform = GVector2(), float angle = 0, GVector2 rotationCenter = GVector2(), D3DXCOLOR colorKey = D3DCOLOR_XRGB(255, 255, 255));
	void draw(GVector2 translate);

	void setPosition(GVector3 pos);
	void setPosition(float x, float y);
	void setPosition(GVector2 pos);

	POINT getAnchorPoint();

	GVector2 getScale();
	void setScale(GVector2 scale);

	float getRotation();
	void setRotation(float rotation); // by radian

	GVector2 getRotationCenter();
	void setRotationCenter(GVector2 rotationCenter);

	void setFlipVertical(bool flag);
	void setFlipHorizontal(bool flag);

	void reset();

	GVector2 getTranslation(); // phep tinh tien tu world position -> view position
	void setTranslation(GVector2 translation); // phep tinh tien: tu the world position -> view position
	Sprite* getSprite();

	void CopyAnimation(Animation* animation);

	void setStop(bool flag);
	void setLoop(bool flag, int start, int stop);
	~Animation();

	int _currentIndex = 1,
		_totalFrame;

protected:
	void _InitWithAnimation(eID eId, string nameAnimation, int totalFrame, float timePerFrame = 0.1f);

	bool _enableStop = false;
	bool _enableLoop = false;
	int _startLoop, _stopLoop = 0;

	int _frameW,
		_frameH;

	float _timePerFrame,
		_currentTotalTime;

	Sprite* _pSprite;

	string _nameAnimation;
	eID _eID;
	RECT _srcRect;
};

#endif
