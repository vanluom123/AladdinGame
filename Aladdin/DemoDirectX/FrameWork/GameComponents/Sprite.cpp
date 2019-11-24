#include "../GameComponents/Sprite.h"

Sprite::Sprite(const char*filePath, RECT sourceRect, int width, int height, D3DCOLOR colorKey)
{
	this->InitWithSprite(filePath, sourceRect, width, height, colorKey);
}

Sprite::Sprite()
{}

Sprite::~Sprite()
{}

void Sprite::InitWithSprite(const char* filePath, RECT sourceRect, int width, int height, D3DCOLOR colorKey)
{
	HRESULT result;
	mSpriteHandler = GameGlobal::GetCurrentSpriteHandler();
	mPosition = GVector3(0, 0, 0);
	mRotation = 0;
	mRotationCenter = GVector2(mPosition.x, mPosition.y);
	mTranslation = GVector2(0, 0);
	mScale = GVector2(0, 1);
	mSourceRect = sourceRect;
	mScale.x = mScale.y = 1;

	D3DXGetImageInfoFromFileA(filePath, &mImageInfo);

	if (width == NULL)
	{
		if (!isRect(sourceRect))
			mWidth = mImageInfo.Width;
		else mWidth = sourceRect.right - sourceRect.left;
	}
	else mWidth = width;

	if (height == NULL)
	{
		if (!isRect(sourceRect))
			mHeight = mImageInfo.Height;
		else
			mHeight = sourceRect.bottom - sourceRect.top;
	}
	else
		mHeight = height;

	if (!isRect(sourceRect))
	{
		mSourceRect.left = 0;
		mSourceRect.right = mWidth;
		mSourceRect.top = 0;
		mSourceRect.bottom = mHeight;
	}

	LP_3DDEVICE device;
	mSpriteHandler->GetDevice(&device);

	D3DXCreateTextureFromFileExA(
		device,
		filePath,
		mImageInfo.Width,
		mImageInfo.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		colorKey,	//NOTE
		&mImageInfo,
		NULL,
		&mTexture);
	POINT tmp; tmp.x = -1; tmp.y = -1;
}

bool Sprite::isRect(RECT rect)
{
	if (rect.left == rect.right)
		return false;

	if (rect.top == rect.bottom)
		return false;

	return true;
}

int Sprite::GetWidth()
{
	return mWidth;
}

void Sprite::SetWidth(int width)
{
	mWidth = width;
}

int Sprite::GetHeight()
{
	return mHeight;
}

void Sprite::SetHeight(int height)
{
	mHeight = height;
}

void Sprite::Draw(GVector3 position, RECT sourceRect, GVector2 scale, GVector2 transform, float angle, GVector2 rotationCenter, D3DXCOLOR colorKey)
{
	GVector3 inPosition = mPosition;
	RECT inSourceRect = mSourceRect;
	float inRotation = mRotation;
	GVector2 inScale = mScale;
	GVector2 inTranslation = mTranslation;
	GVector2 inRotationCenter = mRotationCenter;
	GVector2 scalingCenter = GVector2(inPosition.x, inPosition.y);

	if (position != GVector3())
		inPosition = position;

	if (isRect(sourceRect))
		inSourceRect = sourceRect;

	if (scale != GVector2())
		inScale = scale;

	if (transform != GVector2())
		inTranslation = transform;

	if (rotationCenter != GVector2())
		inRotationCenter = rotationCenter;
	else
		mRotationCenter = GVector2(inPosition.x, inPosition.y);// cho phep quay giua hinh

	D3DXMatrixTransformation2D(&mMatrix, &scalingCenter, 0, &inScale, &inRotationCenter,
		inRotation, &inTranslation);

	D3DXMATRIX oldMatrix;
	mSpriteHandler->GetTransform(&oldMatrix);
	mSpriteHandler->SetTransform(&mMatrix);
	GVector3 center;

	if (isAnchorPoint == true)
	{
		if (anchorPoint.x != -1 && anchorPoint.y != -1)
			center = GVector3(anchorPoint.x, anchorPoint.y, 0);
		else
			center = GVector3(mWidth / 2, mHeight, 0);
	}
	else 
		center = GVector3(mWidth / 2, mHeight / 2, 0);

	mSpriteHandler->Draw(mTexture,
		&inSourceRect,
		&center,
		&inPosition,
		D3DCOLOR_ARGB(255, 255, 255, 255)); // nhung pixel nao co mau trang se duoc to mau nay len

	mSpriteHandler->SetTransform(&oldMatrix); // set lai matrix cu~ de Sprite chi ap dung transfrom voi class nay
}


void Sprite::SetSourceRect(RECT rect)
{
	mSourceRect = rect;
}

LP_3DTEXTURE Sprite::GetTexture()
{
	return mTexture;
}

void Sprite::SetPosition(GVector3 pos)
{
	mPosition = pos;
}

void Sprite::SetPosition(float x, float y)
{
	mPosition = GVector3(x, y, 0);
}

void Sprite::SetPosition(GVector2 pos)
{
	mPosition = GVector3(pos.x, pos.y, 0);
}

GVector3 Sprite::GetPosition()
{
	return mPosition;
}

GVector2 Sprite::GetScale()
{
	return mScale;
}

void Sprite::SetScale(GVector2 scale)
{
	mScale = scale;
}

GVector2 Sprite::GetTranslation()
{
	return mTranslation;
}

void Sprite::SetTranslation(GVector2 translation)
{
	mTranslation = translation;
}

GVector2 Sprite::GetRotationCenter()
{
	return mRotationCenter;
}

void Sprite::SetRotationCenter(GVector2 rotationCenter)
{
	mRotationCenter = rotationCenter;
}

float Sprite::GetRotation()
{
	return mRotation;
}

void Sprite::SetRotation(float rotation)
{
	mRotation = rotation;
}
void Sprite::SetAnchorPoint(POINT pos)
{
	isAnchorPoint = true;
	anchorPoint.x = pos.x;
	anchorPoint.y = pos.y;
}

void Sprite::SetAnchorPoint(float x, float y)
{
	isAnchorPoint = true;
	anchorPoint.x = x;
	anchorPoint.y = y;
}
void Sprite::SetAnchorPoint(bool flag)
{
	isAnchorPoint = flag;
}

POINT Sprite::GetAnchorPoint()
{
	return anchorPoint;
}

IMAGE_INFO Sprite::GetImageInfo()
{
	return mImageInfo;
}

void Sprite::FlipHorizontal(bool flag)
{
	if (mIsFlipHorizontal != flag)
	{
		mIsFlipHorizontal = flag;
		mScale = GVector2(-mScale.x, mScale.y);
	}
}

bool Sprite::isFlipHorizontal()
{
	return mIsFlipHorizontal;
}

void Sprite::FlipVertical(bool flag)
{
	if (mIsFlipVertical != flag)
	{
		mIsFlipVertical = flag;
		mScale = GVector2(mScale.x, -mScale.y);
	}
}

bool Sprite::isFlipVertical()
{
	return mIsFlipVertical;
}
