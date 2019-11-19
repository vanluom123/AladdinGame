#pragma once

#include "GameDefines/GameDefine.h"
#include "Utils/properties.h"

class SpriteSheet
{

private:
	D3DXIMAGE_INFO m_ImageInfo;
	LP_3DTEXTURE m_Texture;
	D3DXMATRIX m_Matrix;

	GVector2 m_pos;
	int m_width;
	int m_Height;
	RECT m_srect;
	GVector2 m_Scaling;
	bool m_IsFlipX;
	bool m_IsFlipY;
	float m_Rotation;
	GVector2 m_Translation;

public:
	SpriteSheet() = default;
	SpriteSheet(const char* filePath, D3DCOLOR color);
	~SpriteSheet();

	void init(const char* filePath, D3DCOLOR color);

	void render(D3DCOLOR color);

	SET(SpriteSheet, float, PositionY, { m_pos.y = value; })
	SET(SpriteSheet, float, PositionX, { m_pos.x = value; })
	GETSET(SpriteSheet, GVector2, Position, { return m_pos; }, { m_pos = value; })

	GETSET(SpriteSheet, int, Height, { return m_Height; }, { m_Height = value; })
	GETSET(SpriteSheet, int, Width, { return m_width; }, { m_width = value; })

	GETSET(SpriteSheet, RECT, Rect, { return m_srect; }, { m_srect = value; })

	GETSET(SpriteSheet, GVector2, Scale, { return m_Scaling; }, { m_Scaling = value; })
	SET(SpriteSheet, float, ScaleX, { m_Scaling.x = value; })
	SET(SpriteSheet, float, ScaleY, { m_Scaling.y = value; })

	GETSET(SpriteSheet, bool, FlipX, { return m_IsFlipX; }, {
		if (m_IsFlipX != value)
		{
			m_IsFlipX = value;
			m_Scaling.x *= -1;
		}
	})

	GETSET(SpriteSheet, bool, FlipY, { return m_IsFlipY; }, {
		if (m_IsFlipY != value)
		{
			m_IsFlipY = value;
			m_Scaling.y *= -1;
		}
	})

	GETSET(SpriteSheet, float, Rotate, { return m_Rotation; }, { m_Rotation = value; })

	GETSET(SpriteSheet, GVector2, Translation, { return m_Translation; }, { m_Translation = value; })
	SET(SpriteSheet, float, TranslationX, { m_Translation.x = value; })
	SET(SpriteSheet, float, TranslationY, { m_Translation.y = value; })
};

