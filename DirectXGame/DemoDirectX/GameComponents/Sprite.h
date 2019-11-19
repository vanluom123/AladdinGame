#ifndef __SPRITE__
#define __SPRITE__

#include "GameGlobal.h"
#include "../GameDefines/GameDefine.h"
#include "../Utils/properties.h"

class Sprite
{

protected:
	void _InitWithSprite(const char* filePath, RECT sourRect = RECT(), int width = 0, int height = 0, D3DCOLOR colorKey = NULL);

	bool _IsRect(RECT rect);

	POINT _anchorPoint;
	bool _enableAnchorPoint = false;

	GVector3 _position;
	LP_3DTEXTURE _pTexture;
	LP_SPRITE _pSpriteHandler;
	IMAGE_INFO _imageInfo;
	RECT _srcRect;

	int _width, _height;

	bool _isFlipHorizontal;
	bool _isFlipVertical;

	float _rotation;

	GVector2 _scale;
	GVector2 _translation;
	D3DXMATRIX _matrix;
	GVector2 _rotationCenter;

public:
	Sprite(const char* filePath, RECT sourceRect = RECT(), int width = NULL, int height = NULL, D3DCOLOR colorKey = NULL);
	Sprite() = default;
	~Sprite() = default;

	void render(GVector3 position = GVector3(), RECT sourceRect = RECT(), GVector2 scale = GVector2(), GVector2 transform = GVector2(), float angle = 0, GVector2 rotationCenter = GVector2(), D3DXCOLOR colorKey = D3DCOLOR_XRGB(255, 255, 255));

	// Property
	GETSET(Sprite, int, Width, { return _width; }, { _width = value; })
	GETSET(Sprite, int, Height, { return _height; }, { _height = value; })

	GET(Sprite, LP_3DTEXTURE, Texture, { return _pTexture; })

	GET(Sprite, D3DXIMAGE_INFO, ImageInfo, { return _imageInfo; })

	SET(Sprite, float, PositionY, { _position.y = value; })
	SET(Sprite, float, PositionX, { _position.x = value; })
	GETSET(Sprite, GVector3, Position, { return _position; }, { _position = value; })

	GETSET(Sprite, GVector2, Scale, { return _scale; }, { _scale = value; })

	GETSET(Sprite, GVector2, Translation, { return _translation; }, { _translation = value; })

	GETSET(Sprite, GVector2, RotationCenter, { return _rotationCenter; }, { _rotationCenter = value; })

	GETSET(Sprite, float, Rotation, { return _rotation; }, { _rotation = value; })

	SET(Sprite, float, AnchorY, { _anchorPoint.y = value; })
	SET(Sprite, float, AnchorX, { _anchorPoint.x = value; })

	SET(Sprite, bool, EnableAnchorPoint, {
		if(_enableAnchorPoint != value)
			_enableAnchorPoint = value;
	})

	GETSET(Sprite, POINT, AnchorPoint, { return _anchorPoint; }, {

		_enableAnchorPoint = true;
		_anchorPoint = value;

	})

	GETSET(Sprite, RECT, SrcRect, { return _srcRect; }, { _srcRect = value; })

	GETSET(Sprite, bool, FlipVertical, { return _isFlipVertical; }, {

		if (_isFlipVertical != value)
		{
			_isFlipVertical = value;
			_scale.y *= -1;
		}
	})
	GETSET(Sprite, bool, FlipHorizontal, { return _isFlipHorizontal; }, {
		if (_isFlipHorizontal != value)
		{
			_isFlipHorizontal = value;
			_scale.x *= -1;
		}
	})

};

#endif