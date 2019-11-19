#include "../GameComponents/Sprite.h"

Sprite::Sprite(const char* filePath, RECT sourceRect, int width, int height, D3DCOLOR colorKey) :
	Width(this),
	Height(this),
	Texture(this),
	ImageInfo(this),
	Position(this),
	PositionX(this),
	PositionY(this),
	Scale(this),
	Translation(this),
	RotationCenter(this),
	Rotation(this),
	AnchorX(this),
	AnchorY(this),
	AnchorPoint(this),
	SrcRect(this),
	FlipHorizontal(this),
	FlipVertical(this),
	EnableAnchorPoint(this)
{
	this->_InitWithSprite(filePath, sourceRect, width, height, colorKey);
}

void Sprite::_InitWithSprite(const char* filePath, RECT sourceRect, int width, int height, D3DCOLOR colorKey)
{
	HRESULT result;
	_pSpriteHandler = GameGlobal::GetCurrentSpriteHandler();
	_position = GVector3(0, 0, 0);
	_rotation = 0;
	_rotationCenter = GVector2(_position.x, _position.y);
	_translation = GVector2(0, 0);
	_scale = GVector2(0, 1);
	_srcRect = sourceRect;
	_scale = GVector2(1, 1);

	D3DXGetImageInfoFromFileA(filePath, &_imageInfo);

	if (width == NULL)
	{
		if (!_IsRect(sourceRect))
			_width = _imageInfo.Width;
		else _width = sourceRect.right - sourceRect.left;
	}
	else _width = width;

	if (height == NULL)
	{
		if (!_IsRect(sourceRect))
			_height = _imageInfo.Height;
		else
			_height = sourceRect.bottom - sourceRect.top;
	}
	else
		_height = height;

	if (!_IsRect(sourceRect))
	{
		_srcRect.left = 0;
		_srcRect.right = _width;
		_srcRect.top = 0;
		_srcRect.bottom = _height;
	}

	LP_3DDEVICE device;
	_pSpriteHandler->GetDevice(&device);

	D3DXCreateTextureFromFileExA(
		device,
		filePath,
		_imageInfo.Width,
		_imageInfo.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		colorKey,	//NOTE
		&_imageInfo,
		NULL,
		&_pTexture);
}

bool Sprite::_IsRect(RECT rect)
{
	if (rect.left == rect.right)
		return false;

	if (rect.top == rect.bottom)
		return false;

	return true;
}

void Sprite::render(GVector3 position, RECT sourceRect, GVector2 scale, GVector2 transform, float angle, GVector2 rotationCenter, D3DXCOLOR colorKey)
{
	GVector3 pPosition = _position;
	RECT pSrcRect = _srcRect;
	float pRotation = _rotation;
	GVector2 pScaling = _scale;
	GVector2 pTranslation = _translation;
	GVector2 pRotationCenter = _rotationCenter;
	GVector2 pScalingCenter = GVector2(pPosition.x, pPosition.y);

	if (position != GVector3())
		pPosition = position;

	if (_IsRect(sourceRect))
		pSrcRect = sourceRect;

	if (scale != GVector2())
		pScaling = scale;

	if (transform != GVector2())
		pTranslation = transform;

	if (rotationCenter != GVector2())
		pRotationCenter = rotationCenter;
	else
		_rotationCenter = GVector2(pPosition.x, pPosition.y);

	D3DXMatrixTransformation2D(
		&_matrix,
		&pScalingCenter,
		0,
		&pScaling,
		&pRotationCenter,
		pRotation,
		&pTranslation);

	D3DXMATRIX oldMatrix;
	_pSpriteHandler->GetTransform(&oldMatrix);
	_pSpriteHandler->SetTransform(&_matrix);
	GVector3 center;

	if (_enableAnchorPoint == true)
	{
		if (_anchorPoint.x != -1 && _anchorPoint.y != -1)
			center = GVector3(_anchorPoint.x, _anchorPoint.y, 0);
		else
			center = GVector3(_width / 2, _height / 2, 0);
	}
	else
		center = GVector3(_width / 2, _height / 2, 0);

	_pSpriteHandler->Draw(
		_pTexture,
		&pSrcRect,
		&center,
		&pPosition,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	_pSpriteHandler->SetTransform(&oldMatrix);
}