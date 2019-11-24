#ifndef __SPRITE__
#define __SPRITE__

#include "GameGlobal.h"

class Sprite
{
public:
	Sprite(const char* filePath, RECT sourceRect = RECT(), int width = NULL, int height = NULL, D3DCOLOR colorKey = NULL);
	Sprite();
	~Sprite();

	LP_3DTEXTURE GetTexture();

	void Draw(GVector3 position = GVector3(), RECT sourceRect = RECT(), GVector2 scale = GVector2(), GVector2 transform = GVector2(), float angle = 0, GVector2 rotationCenter = GVector2(), D3DXCOLOR colorKey = D3DCOLOR_XRGB(255, 255, 255));
	
	void SetWidth(int width);
	int GetWidth();

	void SetHeight(int height);
	int GetHeight();

	IMAGE_INFO GetImageInfo();

	GVector3 GetPosition();
	void SetPosition(GVector3 pos);
	void SetPosition(float x, float y);
	void SetPosition(GVector2 pos);

	GVector2 GetScale();
	void SetScale(GVector2 scale);

	GVector2 GetTranslation();
	void SetTranslation(GVector2 translation);

	GVector2 GetRotationCenter();
	void SetRotationCenter(GVector2 rotationCenter);

	float GetRotation();
	void SetRotation(float rotation);

	void SetAnchorPoint(POINT anchorPoint);
	void SetAnchorPoint(float x, float y);
	void SetAnchorPoint(bool flag);
	POINT GetAnchorPoint();

	void SetSourceRect(RECT rect);

	void FlipHorizontal(bool flag);
	bool isFlipHorizontal();

	void FlipVertical(bool flag);
	bool isFlipVertical();

protected:
	void InitWithSprite(const char* filePath, RECT sourRect = RECT(), int width = NULL, int height = NULL, D3DCOLOR colorKey = NULL);

	bool isRect(RECT rect);

	POINT anchorPoint;
	bool isAnchorPoint = false;
	GVector3 mPosition;
	LP_3DTEXTURE mTexture;
	LP_SPRITE mSpriteHandler;
	IMAGE_INFO mImageInfo;
	RECT mSourceRect;

	int mWidth, mHeight;

	bool mIsFlipHorizontal;
	bool mIsFlipVertical;

	float mRotation;

	GVector2 mScale;
	GVector2 mTranslation;
	D3DXMATRIX mMatrix;
	GVector2 mRotationCenter;
};

#endif