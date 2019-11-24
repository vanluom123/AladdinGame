#ifndef __ANIMATION__
#define __ANIMATION__

#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>

#include "../GameComponents/Sprite.h"
#include "../GameControllers/SpriteManager.h"
using namespace std;


class Animation
{
public:
	//ham ho tro lay animation voi anh co duy nhat 1 hang
	Animation(eID eId, string nameAnimation, int totalFrame, float timePerFrame = 0.1f, D3DCOLOR colorKey = NULL);
	Animation();

	virtual void Update(float dt);
	virtual void Update(float dt, bool isDecrease);
	void Draw(GVector3 position = GVector3(), RECT sourceRect = RECT(), GVector2 scale = GVector2(), GVector2 transform = GVector2(), float angle = 0, GVector2 rotationCenter = GVector2(), D3DXCOLOR colorKey = D3DCOLOR_XRGB(255, 255, 255));
	void Draw(GVector2 translate);

	virtual void SetPosition(GVector3 pos);
	virtual void SetPosition(float x, float y);
	virtual void SetPosition(GVector2 pos);

	POINT GetAnchorPoint();

	GVector2 GetScale();
	void SetScale(GVector2 scale);

	float GetRotation();
	void SetRotation(float rotation); // by radian

	GVector2 GetRotationCenter();
	void SetRotationCenter(GVector2 rotationCenter);

	void SetFlipVertical(bool flag);
	void SetFlipHorizontal(bool flag);

	void Reset();

	GVector2 GetTranslation(); // phep tinh tien tu world position -> view position
	void SetTranslation(GVector2 translation); // phep tinh tien: tu the world position -> view position
	Sprite* GetSprite();

	void CopyAnimation(Animation* animation);

	void SetStop(bool flag);
	void SetLoop(bool flag, int start, int stop);
	~Animation();

	int mCurrentIndex = 1; //gia tri frame hien tai - bat dau tu 1 -> tong so frame 
	int	mTotalFrame;//tong so frame

protected:
	//su dung cho ke thua
	void InitWithAnimation(eID eId, string nameAnimation, int totalFrame, float timePerFrame = 0.1f, D3DCOLOR colorKey = NULL);

	bool isStop = false;
	bool isLoop = false;
	int startLoop;
	int stopLoop = 0;

	int mFrameWidth; // chieu rong cua 1 frame 
	int mFrameHeight; // chieu dai cua 1 frame  

	float mTimePerFrame; //thoi gian luan chuyen 1 frame
	float mCurrentTotalTime; //tong thoi gian hien tai de thuc hien timeperframe

	Sprite* mSprite;

	string mNameAnimation;
	eID meId;
	RECT mRect;
};

#endif
